#include "../../helpers/module.h"
#include <chrono>

using namespace Helpers;

static uint32_t GetTimestamp()
{
    return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

static ModuleExtension utilExtension("util", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_GLOBAL_FUNC("uint GetTimestamp()", GetTimestamp, "Gets the current timestamp");
});
