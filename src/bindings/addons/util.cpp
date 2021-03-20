#include "../../helpers/module.h"
#include <chrono>

using namespace Helpers;

static uint64_t GetTimestamp()
{
    return (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

static ModuleExtension utilExtension("util", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_GLOBAL_FUNC("uint64 GetTimestamp()", GetTimestamp, "Gets the current timestamp");
});
