#include "resource.h"
#include "runtime.h"
#include "helpers/module.h"
#include "helpers/events.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "helpers/convert.h"

bool AngelScriptResource::Start()
{
    // Load file
    auto src = ReadFile(resource->GetMain());

    // Compile file
    CScriptBuilder builder;

    builder.SetIncludeCallback(Helpers::IncludeHandler, this);
    builder.SetPragmaCallback(Helpers::PragmaHandler, this);

    int r = builder.StartNewModule(runtime->GetEngine(), resource->GetName().CStr());
    CHECK_AS_RETURN("Builder start", r, false);
    
    r = builder.AddSectionFromMemory(resource->GetMain().CStr(), src.CStr(), src.GetSize());
    CHECK_AS_RETURN("Adding section", r, false);

    r = builder.BuildModule();
    CHECK_AS_RETURN("Compilation", r, false);

    // Start script
    module = builder.GetModule();
    context = runtime->GetEngine()->CreateContext();
    context->SetUserData(this);

    // Get metadata (returns start function)
    asIScriptFunction* func = RegisterMetadata(builder);

    // Get the global start function if no script class start function was found
    if(func == nullptr) func = module->GetFunctionByDecl("void Start()");
    // If main function was still not found, return an error
    if(func == nullptr)
    {
        Log::Error << "The main entrypoint ('void Start()') was not found" << Log::Endl;
        module->Discard();
        context->Release();
        return false;
    }
    r = context->Prepare(func);
    CHECK_AS_RETURN("Context prepare", r, false);

    // Execute script
    r = context->Execute();
    switch(r)
    {
        case asEXECUTION_EXCEPTION:
        {
            Log::Error << "An exception occured while executing the script. Exception: " << Log::Endl;
            Log::Error << GetExceptionInfo(context, alt::ICore::Instance().IsDebug()) << Log::Endl;
            break;
        }
    }

    return true;
}

alt::String AngelScriptResource::ReadFile(alt::String path)
{
    auto pkg = resource->GetPackage();
    // Check if file exists
    if(!pkg->FileExists(path)) return alt::String();
    // Open file
    alt::IPackage::File* pkgFile = pkg->OpenFile(path);
    alt::String src(pkg->GetFileSize(pkgFile));
    // Read file content
    pkg->ReadFile(pkgFile, src.GetData(), src.GetSize());
    pkg->CloseFile(pkgFile);

    return src;
}

bool AngelScriptResource::Stop()
{
    // Gets Stop function and if exists calls it
    if(module != nullptr)
    {
        asIScriptFunction* func = module->GetFunctionByDecl("void Stop()");
        if(func != 0 && context != nullptr)
        {
            auto r = context->Prepare(func);
            CHECK_AS_RETURN("Stop function call", r, false);

            context->Execute();
        }
        module->Discard();
    }

    if(context != nullptr) context->Release();

    // Release the event handler script functions to not create a memory leak
    for(auto pair : eventHandlers)
    {
        pair.second->Release();
    }
    eventHandlers.clear();

    for(auto kv : customLocalEventHandlers)
    {
        kv.second->Release();
    }
    customLocalEventHandlers.clear();

    for(auto kv : customRemoteEventHandlers)
    {
        kv.second->Release();
    }
    customRemoteEventHandlers.clear();

    return true;
}

bool AngelScriptResource::OnEvent(const alt::CEvent* ev)
{
    if(ev->GetType() == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
    {
        HandleCustomEvent(ev, true);
        return true;
    }
    else if(ev->GetType() == alt::CEvent::Type::CLIENT_SCRIPT_EVENT)
    {
        HandleCustomEvent(ev, false);
        return true;
    }
    // Get the handler for the specified event
    auto event = Helpers::Event::GetEvent(ev->GetType());
    if(event == nullptr)
    {
        Log::Error << "Unhandled event type " << std::to_string((uint16_t)ev->GetType()) << Log::Endl;
        return true;
    }
    // Get all script callbacks for the event
    auto callbacks = GetEventHandlers(ev->GetType());
    // Get the args for the event
    auto args = event->GetArgs(this, ev);
    auto returnType = event->GetReturnType();
    // If the return type of the event is bool, it should return a value
    bool shouldReturn = strcmp(returnType, "bool") == 0;

    // Loop over all script callbacks and call them with the args
    for(auto callback : callbacks)
    {
        auto r = context->Prepare(callback);
        CHECK_AS_RETURN("Prepare event handler", r, true);
        for(int i = 0; i < args.size(); i++)
        {
            auto arg = args[i];
            if(arg.second == true) context->SetArgAddress(i, arg.first);
            else context->SetArgObject(i, arg.first);
        }
        r = context->Execute();
        CHECK_AS_RETURN("Execute event handler", r, true);
        if(r == asEXECUTION_FINISHED && shouldReturn)
        {
            auto result = context->GetReturnByte();
            return result == 1 ? true : false;
        }
    }

    return true;
}

void AngelScriptResource::HandleCustomEvent(const alt::CEvent* event, bool local)
{
    std::string name;
    alt::MValueArgs args;
    if(local)
    {
        auto ev = static_cast<const alt::CServerScriptEvent*>(event);
        name = ev->GetName().ToString();
        args = ev->GetArgs();
        CScriptArray* array = runtime->CreateAnyArray(args.GetSize());
        std::vector<asIScriptFunction*> handlers = GetCustomEventHandlers(name, true);
        for(int i = 0; i < args.GetSize(); i++)
        {
            CScriptAny* any = new CScriptAny(runtime->GetEngine());
            auto arg = args[i];
            std::pair<int, void*> converted = Helpers::MValueToValue(runtime, arg);
            any->Store(converted.second, converted.first);
            array->SetValue(i, (void*)any);
            // Free the memory again
            if(converted.first != runtime->GetBaseObjectTypeId()) delete converted.second;
        }
        for(auto handler : handlers)
        {
            auto r = context->Prepare(handler);
            CHECK_AS_RETURN("Prepare custom event handler", r);
            context->SetArgObject(0, array);
            r = context->Execute();
            CHECK_AS_RETURN("Execute custom event handler", r);
        }
    }
    else
    {
        alt::Ref<alt::IPlayer> player;
        auto ev = static_cast<const alt::CClientScriptEvent*>(event);
        name = ev->GetName().ToString();
        args = ev->GetArgs();
        player = ev->GetTarget();
        CScriptArray* array = runtime->CreateAnyArray(args.GetSize());
        std::vector<asIScriptFunction*> handlers = GetCustomEventHandlers(name, false);
        for(int i = 0; i < args.GetSize(); i++)
        {
            CScriptAny* any = new CScriptAny(runtime->GetEngine());
            auto arg = args[i];
            std::pair<int, void*> converted = Helpers::MValueToValue(runtime, arg);
            any->Store(converted.second, converted.first);
            array->SetValue(i, (void*)any);
            // Free the memory again
            if(converted.first != runtime->GetBaseObjectTypeId()) delete converted.second;
        }
        for(auto handler : handlers)
        {
            auto r = context->Prepare(handler);
            CHECK_AS_RETURN("Prepare custom event handler", r);
            context->SetArgObject(0, player.Get());
            context->SetArgObject(1, array);
            r = context->Execute();
            CHECK_AS_RETURN("Execute custom event handler", r);
        }
    }
}

void AngelScriptResource::OnTick()
{
    // Remove all invalid timers
    for (auto &id : invalidTimers) timers.erase(id);
    invalidTimers.clear();

    // Update timers
    for(auto& timer : timers)
    {
        int64_t time = GetTime();
        if(!timer.second->Update(time))
        {
            RemoveTimer(timer.first);
        }
    }
}

asIScriptFunction* AngelScriptResource::RegisterMetadata(CScriptBuilder& builder)
{
    asIScriptFunction* mainFunc = nullptr;
    // todo: add support meta properly
    /*
    uint32_t count = module->GetObjectTypeCount();
    for(uint32_t i = 0; i < count; i++)
    {
        // Get the type for the class
        auto type = module->GetObjectTypeByIndex(i);
        // Get metadata for the class
        std::vector<std::string> metadata = builder.GetMetadataForType(type->GetTypeId());
        for(auto meta : metadata)
        {
            // The metadata equals IServer so its our main server class
            if(meta == "IServer")
            {
                // Get all methods and check their metadata
                auto methods = type->GetMethodCount();
                for(uint32_t n = 0; i < methods; i++)
                {
                    auto method = type->GetMethodByIndex(n, false);
                    // Get metadata for the method
                    std::vector<std::string> methodMetas = builder.GetMetadataForTypeMethod(type->GetTypeId(), method);
                    for(auto methodMeta : methodMetas)
                    {
                        if(methodMeta == "Start") mainFunc = method;
                    }
                }
            }
        }
    }
    */

    return mainFunc;
}
