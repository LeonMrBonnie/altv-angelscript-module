#pragma warning(disable : 4003)
#include "resource.h"
#include "runtime.h"
#include "helpers/module.h"
#include "helpers/events.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "helpers/convert.h"
#include "./helpers/benchmark.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define IS_WINDOWS
#include "windows.h"
#include "./helpers/bytecode.h"
#define FILE_SEPERATOR "\\"
#else
#define IS_LINUX
#define FILE_SEPERATOR "/"
#endif

bool AngelScriptResource::Start()
{
    #ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("ResourceStart_" + resource->GetName().ToString());
    #endif

    // Get main filename and extension
    std::string main = resource->GetMain().ToString();
    std::string extension = main.substr(main.find_last_of(".") + 1);
    std::string fileName = main.substr(0, main.find_last_of("."));

    // Load file
    auto src = ReadFile(resource->GetMain());

    // Compile file
    CScriptBuilder builder;

    builder.SetIncludeCallback(Helpers::IncludeHandler, this);
    builder.SetPragmaCallback(Helpers::PragmaHandler, this);

    int r = builder.StartNewModule(runtime->GetEngine(), resource->GetName().CStr());
    CHECK_AS_RETURN("Builder start", r, false);

    module = builder.GetModule();

    // Check if the file is precompiled binary code
    if(extension == "asb")
    {
        BytecodeStream byteStream(resource->GetPath().ToString() + FILE_SEPERATOR + main, true);
        if(byteStream.HasErrored())
        {
            Log::Error << "Failed to open bytecode file" << Log::Endl;
            return false;
        }
        r = module->LoadByteCode(&byteStream, false);
        CHECK_AS_RETURN("Load bytecode", r, false);
        byteStream.Close();
    }
    else
    {
        r = builder.AddSectionFromMemory(main.c_str(), src.CStr(), src.GetSize());
        CHECK_AS_RETURN("Adding section", r, false);

        r = builder.BuildModule();
        CHECK_AS_RETURN("Compilation", r, false);
    }

    // Check if compile only mode
    #ifdef IS_WINDOWS
    std::string commandline(GetCommandLineA());
    if(commandline.find("--save-bytecode") != std::string::npos && extension != "asb")
    {
        Helpers::BytecodeStream byteStream(resource->GetPath().ToString() + FILE_SEPERATOR + fileName + ".asb");
        if(byteStream.HasErrored())
        {
            Log::Error << "Failed to open bytecode file" << Log::Endl;
            return false;
        }
        r = module->SaveByteCode(&byteStream, false);
        byteStream.Close();
        CHECK_AS_RETURN("Save bytecode", r, false);
        Log::Colored << "~g~Successfully saved the compiled bytecode to the file ~w~" << fileName + ".asb" << Log::Endl;

        return true;
    }
    #endif

    RegisterImports();

    // Start script
    context = runtime->GetEngine()->CreateContext();
    context->SetUserData(this);

    // Get metadata (returns start function)
    asIScriptFunction* func = RegisterMetadata(builder, context);

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
    context->Unprepare();

    return true;
}

void AngelScriptResource::RegisterImports()
{
    for(uint32_t i = 0; i < module->GetImportedFunctionCount(); i++)
    {
        auto source = module->GetImportedFunctionSourceModule(i);
        auto decl = module->GetImportedFunctionDeclaration(i);

        // Get the resource and check if its started
        auto sourceResource = alt::ICore::Instance().GetResource(source);
        if(!sourceResource || !sourceResource->IsStarted())
        {
            Log::Error << "Could not bind function '" << decl << "' from module '" << source << "'" << Log::Endl;
            return;
        }

        // The resource is a angelscript resource
        if(sourceResource->GetType() == MODULE_TYPE)
        {
            auto sourceModule = runtime->GetEngine()->GetModule(sourceResource->GetName().CStr());
            auto func = sourceModule->GetFunctionByDecl(decl);
            if(func == nullptr)
            {
                Log::Error << "Could not bind function '" << decl << "' from module '" << source << "'" << Log::Endl;
                return;
            }
            module->BindImportedFunction(i, func);
        }
        // The resource is not a angelscript resource
        else
        {
            // todo: add import of functions from non angelscript resources
            /*auto exports = sourceResource->GetExports();
            for(auto it = exports->Begin(); exports->Next();)
            {
                auto name = it->GetKey();
                auto func = it->GetValue().As<alt::MValueFunctionConst>();
            }*/
        }
    }
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
    #ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("OnEvent_" + resource->GetName().ToString() + "_" + std::string(std::to_string((uint16_t)ev->GetType())));
    #endif

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
    auto returnType = event->GetReturnType();
    // If the return type of the event is bool, it should return a value
    bool shouldReturn = strcmp(returnType, "bool") == 0;

    // Loop over all script callbacks and call them with the args
    for(auto callback : callbacks)
    {
        auto r = context->Prepare(callback);
        CHECK_AS_RETURN("Prepare event handler", r, true);
        // Set the event args and execute callback
        r = event->Execute(this, ev);
        CHECK_AS_RETURN("Execute event handler", r, true);
        if(r == asEXECUTION_FINISHED && shouldReturn)
        {
            auto result = context->GetReturnByte();
            context->Unprepare();
            return result == 1 ? true : false;
        }
        context->Unprepare();
    }

    // Check if the main script class has been set
    if(mainScriptClass != nullptr)
    {
        // Get the method of the main script class for the event if it exists
        auto type = mainScriptClass->GetObjectType();
        asIScriptFunction* eventFunc = nullptr;
        for(asUINT i = 0; i < type->GetMethodCount(); i++)
        {
            auto func = type->GetMethodByIndex(i, false);
            auto data = func->GetUserData();
            if(data == nullptr) continue;

            Event* eventData = static_cast<Event*>(data);
            if(eventData->GetType() != ev->GetType()) continue;

            eventFunc = func;
            break;
        }
        if(eventFunc != nullptr)
        {
            int r = context->Prepare(eventFunc);
            CHECK_AS_RETURN("Prepare main script class event method", r, true);
            r = context->SetObject(mainScriptClass);
            CHECK_AS_RETURN("Set main script class event method object", r, true);
            r = event->Execute(this, ev);
            CHECK_AS_RETURN("Execute main script class event method", r, true);
            if(r == asEXECUTION_FINISHED && shouldReturn)
            {
                auto result = context->GetReturnByte();
                context->Unprepare();
                return result == 1 ? true : false;
            }
            context->Unprepare();
        }
    }

    return true;
}

void AngelScriptResource::HandleCustomEvent(const alt::CEvent* event, bool local)
{
    #ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("HandleCustomEvent_" + resource->GetName().ToString() + "_" + static_cast<const alt::CServerScriptEvent*>(event)->GetName().ToString());
    #endif

    std::string name;
    alt::MValueArgs args;
    if(local)
    {
        auto ev = static_cast<const alt::CServerScriptEvent*>(event);
        name = ev->GetName().ToString();
        args = ev->GetArgs();
        std::vector<asIScriptFunction*> handlers = GetCustomEventHandlers(name, true);
        if(handlers.size() == 0) return;

        alt::Array<std::tuple<int, void*>> handlerArgs;
        for(auto arg : args)
        {
            std::tuple<int, void*> converted = Helpers::MValueToValue(runtime, arg);
            handlerArgs.Push(converted);
        }

        for(auto handler : handlers)
        {
            auto r = context->Prepare(handler);
            CHECK_AS_RETURN("Prepare custom event handler", r);
            for(int i = 0; i < handlerArgs.GetSize(); i++)
            {
                auto [typeId, ptr] = handlerArgs[i];
                int ret;
                if(Helpers::IsTypePrimitive(typeId)) ret = context->SetArgAddress(i, ptr);
                else ret = context->SetArgObject(i, ptr);
                CHECK_AS_RETURN("Set custom event handler arg", ret);
            }
            r = context->Execute();
            CHECK_AS_RETURN("Execute custom event handler", r);
        }

        context->Unprepare();
        for(auto [typeId, ptr] : handlerArgs)
        {
            if(typeId != runtime->GetBaseObjectTypeId()) delete ptr;
        }
    }
    else
    {
        alt::Ref<alt::IPlayer> player;
        auto ev = static_cast<const alt::CClientScriptEvent*>(event);
        name = ev->GetName().ToString();
        args = ev->GetArgs();
        player = ev->GetTarget();
        std::vector<asIScriptFunction*> handlers = GetCustomEventHandlers(name, false);
        if(handlers.size() == 0) return;

        alt::Array<std::tuple<int, void*>> handlerArgs;
        for(auto arg : args)
        {
            std::tuple<int, void*> converted = Helpers::MValueToValue(runtime, arg);
            handlerArgs.Push(converted);
        }

        for(auto handler : handlers)
        {
            auto r = context->Prepare(handler);
            CHECK_AS_RETURN("Prepare custom event handler", r);
            context->SetArgObject(0, player.Get());
            for(int i = 0; i < handlerArgs.GetSize(); i++)
            {
                auto [typeId, ptr] = handlerArgs[i];
                int ret;
                if(Helpers::IsTypePrimitive(typeId)) ret = context->SetArgAddress(i + 1, ptr);
                else ret = context->SetArgObject(i + 1, ptr);
                CHECK_AS_RETURN("Set custom event handler arg", ret);
            }
            r = context->Execute();
            CHECK_AS_RETURN("Execute custom event handler", r);
        }

        context->Unprepare();
        for(auto [typeId, ptr] : handlerArgs)
        {
            if(typeId != runtime->GetBaseObjectTypeId()) delete ptr;
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

asIScriptFunction* AngelScriptResource::RegisterMetadata(CScriptBuilder& builder, asIScriptContext* context)
{
    asIScriptFunction* mainFunc = nullptr;
    
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
            if(meta != "IServer") continue;

            // Create an instance
            auto factory = type->GetFactoryByIndex(0);
            context->Prepare(factory);
            context->Execute();
            asIScriptObject* obj = *(asIScriptObject**)context->GetAddressOfReturnValue();
            obj->AddRef();
            // Store our instance on the resource
            mainScriptClass = obj;

            // Get all methods and check their metadata
            for(uint32_t n = 0; n < type->GetMethodCount(); n++)
            {
                auto method = type->GetMethodByIndex(n, false);
                // Get metadata for the method
                std::vector<std::string> methodMetas = builder.GetMetadataForTypeMethod(type->GetTypeId(), method);

                for(auto methodMeta : methodMetas)
                {
                    Log::Info << methodMeta << Log::Endl;
                    // Get the event associated with the metadata
                    Event* event = Event::GetByMetadata(methodMeta);
                    if(event == nullptr) continue;
                    // Store the event on the method
                    method->SetUserData(event);
                }
            }

            // We are done so we break out of all the loops and return
            goto done;
        }
    }

done:
    return mainFunc;
}
