#include "main.h"
#include "Log.h"
#include "./helpers/benchmark.h"
#include "./helpers/angelscript.h"
#include "angelscript/include/angelscript.h"

#ifdef DEBUG_MEMORY
extern bool Helpers::showAllocationMessages = false;
#endif

#define DEBUG_USAGE_MSG Log::Colored << "~y~Usage: ~w~as-module [general/resource] [resourceName?]" << Log::Endl
void CommandHandler(alt::Array<alt::StringView> args, void* userData)
{
    auto argCount = args.GetSize();
    Log::Colored << "~g~[[[ ~w~AngelScript Module ~g~]]]" << Log::Endl;
    if(argCount == 0 || args[0] == "--help")
    {
        Log::Colored << "Available commands:" << Log::Endl;
        Log::Colored << "~y~--help ~w~| Shows the command help" << Log::Endl;
        Log::Colored << "~y~--info ~w~| Shows some debug info" << Log::Endl;
        Log::Colored << "~y~--credits ~w~| Shows the module credits" << Log::Endl;
        Log::Colored << "~y~--debug ~w~| Shows debug info" << Log::Endl;
    }
    else if(args[0] == "--info")
    {
        Log::Colored << "Module version: ~y~" MODULE_VERSION << Log::Endl;
        Log::Colored << "SDK version: ~y~v" << alt::ICore::SDK_VERSION << Log::Endl;
        Log::Colored << "AngelScript version: ~y~v" << asGetLibraryVersion() << Log::Endl;
    }
    else if(args[0] == "--credits")
    {
        Log::Colored << "~y~LeonMrBonnie ~w~| Creator of the module" << Log::Endl;
        Log::Colored << "~y~Andreas Jönsson ~w~| Creator of AngelScript" << Log::Endl;
        Log::Colored << "~y~Vektor aka. voidlord ~w~| General help with the module" << Log::Endl;
        Log::Colored << "~y~bluecataudio ~w~& ~y~BlindMindStudios ~w~| Creators of the AngelScript JIT Compiler" << Log::Endl;
    }
    else if(args[0] == "--debug")
    {
        if(!alt::ICore::Instance().IsDebug())
        {
            Log::Error << "Debug info is only available in debug mode" << Log::Endl;
            return;
        }
        if(argCount < 2)
        {
            DEBUG_USAGE_MSG;
            return;
        }

        auto type = args[1];
        if(type == "general")
        {
            // Show general debug info
            AngelScriptRuntime::Instance().ShowDebugInfo();
        }
        else if(type == "resource")
        {
            if(argCount < 3)
            {
                DEBUG_USAGE_MSG;
                return;
            }
            // Show resource specific debug info
            auto                 iresource = alt::ICore::Instance().GetResource(args[2]);
            AngelScriptResource* resource  = nullptr;
            if(iresource != nullptr) resource = AngelScriptRuntime::Instance().GetResource(iresource);
            if(resource == nullptr)
            {
                Log::Error << "Invalid resource '" << args[2] << "'" << Log::Endl;
                return;
            }
            resource->ShowDebugInfo();
        }
    }
#ifdef DEBUG_MEMORY
    else if(args[0] == "--allocations")
    {
        std::string state;
        if(Helpers::showAllocationMessages)
        {
            Helpers::showAllocationMessages = false;
            state                           = "~lr~Disabled";
        }
        else
        {
            Helpers::showAllocationMessages = true;
            state                           = "~lg~Enabled";
        }
        Log::Colored << state << "~w~ memory allocation messages" << Log::Endl;
    }
#endif
    else
    {
        Log::Warning << "Option '" << args[0] << "' does not exist" << Log::Endl;
    }
}

#ifdef SERVER_MODULE
EXPORT bool altMain(alt::ICore* core)
{
    #ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("Main_Init");
    #endif

    alt::ICore::SetInstance(core);

    // Create instance of runtime and register
    auto& runtime = AngelScriptRuntime::Instance();
    core->RegisterScriptRuntime(MODULE_TYPE, &runtime);
    Log::Colored << "Loaded AngelScript module. Version ~y~" MODULE_VERSION << Log::Endl;

    // Register command
    core->SubscribeCommand("angelscript-module", CommandHandler);
    core->SubscribeCommand("as-module", CommandHandler);

    return true;
}
#endif
#ifdef CLIENT_MODULE
EXPORT void CreateScriptRuntime(alt::ICore* core)
{
    #ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("Main_Init");
    #endif

    alt::ICore::SetInstance(core);

    // Create instance of runtime and register
    auto& runtime = AngelScriptRuntime::Instance();
    core->RegisterScriptRuntime(MODULE_TYPE, &runtime);
    Log::Colored << "Loaded AngelScript client module. Version ~y~" MODULE_VERSION << Log::Endl;

    // Register command
    core->SubscribeCommand("angelscript-module", CommandHandler);
    core->SubscribeCommand("as-module", CommandHandler);
}

EXPORT const char* GetType()
{
    return MODULE_TYPE;
}
#endif

EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}
