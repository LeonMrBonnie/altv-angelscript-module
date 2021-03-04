#include "main.h"
#include "Log.h"
#include "./helpers/benchmark.h"
#include "angelscript/include/angelscript.h"

void CommandHandler(alt::Array<alt::StringView> args, void* userData)
{
    Log::Colored << "~g~[[[ ~w~AngelScript Module ~g~]]]" << Log::Endl;
    if(args.GetSize() == 0 || args[0] == "--help")
    {
        Log::Colored << "Available commands:" << Log::Endl;
        Log::Colored << "~y~--help ~w~| Shows the command help" << Log::Endl;
        Log::Colored << "~y~--info ~w~| Shows some debug info" << Log::Endl;
        Log::Colored << "~y~--credits ~w~| Shows the module credits" << Log::Endl;
    }
    else if(args[0] == "--info")
    {
        Log::Colored << "Module version: ~y~v" MODULE_VERSION << Log::Endl;
        Log::Colored << "SDK version: ~y~v" << alt::ICore::SDK_VERSION << Log::Endl;
        Log::Colored << "AngelScript version: ~y~v" << asGetLibraryVersion() << Log::Endl;
    }
    else if(args[0] == "--credits")
    {
        Log::Colored << "~y~LeonMrBonnie ~w~| Creator of the module" << Log::Endl;
        Log::Colored << "~y~Andreas Jönsson ~w~| Creator of AngelScript" << Log::Endl;
        Log::Colored << "~y~Vektor aka. voidlord ~w~| General help with the module" << Log::Endl;
    }
}

EXPORT bool altMain(alt::ICore* core)
{
    #ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("Main_Init");
    #endif

    alt::ICore::SetInstance(core);

    // Create instance of runtime and register
    auto& runtime = AngelScriptRuntime::Instance();
    core->RegisterScriptRuntime(MODULE_TYPE, &runtime);
    Log::Colored << "Loaded AngelScript module. Version ~y~v" MODULE_VERSION << Log::Endl;

    // Register command
    core->SubscribeCommand("angelscript-module", CommandHandler);
    core->SubscribeCommand("as-module", CommandHandler);

    return true;
}

EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}
