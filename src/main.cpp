#include "main.h"
#include "Log.h"
#include "./helpers/benchmark.h"

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

    return true;
}

EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}
