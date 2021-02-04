#include "resource.h"
#include "runtime.h"
#include "angelscript/addon/scriptbuilder/scriptbuilder.h"
#include "angelscript/addon/scripthelper/scripthelper.h"

#define CHECK_AS_RETURN(type, result) \
    if(r < 0) \
    { \
        Log::Error << type << " error. Error code: " << std::to_string(result) << Log::Endl;\
        return false;\
    }

static int Include(const char* include, const char* from, CScriptBuilder* builder, void* data)
{
    // todo: add support for relative paths
    auto resource = static_cast<AngelScriptResource*>(data);
    auto src = resource->ReadFile(alt::String(include));
    int r = builder->AddSectionFromMemory(include, src.CStr(), src.GetSize());
    CHECK_AS_RETURN("Include", r);
}

bool AngelScriptResource::Start()
{
    // Load file
    auto src = ReadFile(resource->GetMain());

    // Compile file
    CScriptBuilder builder;

    builder.SetIncludeCallback(Include, this);

    int r = builder.StartNewModule(runtime->GetEngine(), resource->GetName().CStr());
    CHECK_AS_RETURN("Builder start", r);
    
    r = builder.AddSectionFromMemory(resource->GetMain().CStr(), src.CStr(), src.GetSize());
    CHECK_AS_RETURN("Adding section", r);

    r = builder.BuildModule();
    CHECK_AS_RETURN("Compilation", r);

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
    CHECK_AS_RETURN("Context prepare", r);

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
    alt::IPackage::File* pkgFile = pkg->OpenFile(resource->GetMain());
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
        CHECK_AS_RETURN("Stop function call", r);

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
