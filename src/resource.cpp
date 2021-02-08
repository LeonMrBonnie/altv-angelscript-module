#include "resource.h"
#include "runtime.h"
#include "angelscript/addon/scriptbuilder/scriptbuilder.h"
#include "angelscript/addon/scripthelper/scripthelper.h"
#include "helpers/module.h"
#include "helpers/events.h"

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

    asIScriptFunction* func = module->GetFunctionByDecl("void Start()");
    if(func == 0)
    {
        Log::Error << "The main entrypoint was not found" << Log::Endl;
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

    RegisterTypeInfos();

    return true;
}

alt::String AngelScriptResource::ReadFile(alt::String path)
{
    // Reads file content
    auto pkg = resource->GetPackage();
    alt::IPackage::File* pkgFile = pkg->OpenFile(path);
    alt::String src(pkg->GetFileSize(pkgFile));
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

    UnregisterTypeInfos();

    return true;
}

bool AngelScriptResource::OnEvent(const alt::CEvent* ev)
{
    auto event = Helpers::Event::GetEvent(ev->GetType());
    if(event == nullptr)
    {
        Log::Error << "Unhandled event type " << std::to_string((uint16_t)ev->GetType()) << Log::Endl;
        return true;
    }
    auto callbacks = GetEventHandlers(ev->GetType());
    auto args = event->GetArgs(this, ev);
    auto returnType = event->GetReturnType();
    bool shouldReturn = strcmp(returnType, "bool") == 0;

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

void AngelScriptResource::OnTick()
{
    for (auto &id : invalidTimers) timers.erase(id);
    invalidTimers.clear();

    for(auto& timer : timers)
    {
        int64_t time = GetTime();
        if(!timer.second->Update(time))
        {
            RemoveTimer(timer.first);
        }
    }
}

void AngelScriptResource::RegisterTypeInfos()
{
    arrayStringTypeInfo = module->GetTypeInfoByDecl("array<string>");
    arrayStringTypeInfo->AddRef();
    arrayIntTypeInfo = module->GetTypeInfoByDecl("array<int>");
    arrayIntTypeInfo->AddRef();
    arrayUintTypeInfo = module->GetTypeInfoByDecl("array<uint>");
    arrayUintTypeInfo->AddRef();
}

void AngelScriptResource::UnregisterTypeInfos()
{
    if(arrayStringTypeInfo != nullptr) arrayStringTypeInfo->Release();
    if(arrayIntTypeInfo != nullptr) arrayIntTypeInfo->Release();
    if(arrayUintTypeInfo != nullptr) arrayUintTypeInfo->Release();
}

CScriptArray* AngelScriptResource::CreateStringArray(uint32_t len)
{
    auto arr = CScriptArray::Create(arrayStringTypeInfo, len);
    return arr;
}

CScriptArray* AngelScriptResource::CreateIntArray(uint32_t len)
{
    auto arr = CScriptArray::Create(arrayIntTypeInfo, len);
    return arr;
}

CScriptArray* AngelScriptResource::CreateUIntArray(uint32_t len)
{
    auto arr = CScriptArray::Create(arrayUintTypeInfo, len);
    return arr;
}
