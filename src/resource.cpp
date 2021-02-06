#include "resource.h"
#include "runtime.h"
#include "angelscript/addon/scriptbuilder/scriptbuilder.h"
#include "angelscript/addon/scripthelper/scripthelper.h"
#include "helpers/module.h"

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
    asIScriptFunction* func = module->GetFunctionByDecl("void Stop()");
    if(func != 0)
    {
        auto r = context->Prepare(func);
        CHECK_AS_RETURN("Stop function call", r, false);

        context->Execute();
    }

    // Cleanup
    module->Discard();
    context->Release();

    return true;
}

bool AngelScriptResource::OnEvent(const alt::CEvent* ev)
{
    // todo: event handling
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
