#include "../../helpers/module.h"
#include "../../helpers/angelscript.h"
#include "angelscript/addon/scriptbuilder/scriptbuilder.h"
#include <chrono>

using namespace Helpers;

static uint64_t GetTimestamp()
{
    return (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

static void ShowCallstack(uint32_t maxLevels = 0)
{
    Helpers::PrintCallstack(asGetActiveContext(), maxLevels);
}

static bool Eval(const std::string& code)
{
    GET_RESOURCE();
    if(alt::ICore::Instance().IsDebug() == false)
    {
        THROW_ERROR("Eval is only available in debug mode");
        return false;
    }
    return resource->Eval(code);
}

static ModuleExtension utilExtension("util", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_GLOBAL_FUNC("uint64 GetTimestamp()", GetTimestamp, "Gets the current timestamp");
    REGISTER_GLOBAL_FUNC("void ShowCallstack(uint maxLevels = 0)", ShowCallstack, "Prints the current callstack for debugging");
    REGISTER_GLOBAL_FUNC("bool Eval(const string&in code)", Eval, "Evals the given code");
});
