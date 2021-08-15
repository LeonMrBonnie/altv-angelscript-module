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
    AS_ASSERT(alt::ICore::Instance().IsDebug(), "Eval is only available in debug mode", false);
    return resource->Eval(code);
}

static inline bool replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos) return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
static void Format(asIScriptGeneric* gen)
{
    GET_RESOURCE();
    void*       ref = gen->GetArgAddress(0);
    std::string fmt = *static_cast<std::string*>(ref);

    for(int i = 1, typeId; i < gen->GetArgCount(); i++)
    {
        ref    = gen->GetArgAddress(i);
        typeId = gen->GetArgTypeId(i);
        if(typeId & asTYPEID_OBJHANDLE)
        {
            // We're receiving a reference to the handle, so we need to dereference it
            ref = *(void**)ref;
            resource->GetRuntime()->GetEngine()->AddRefScriptObject(ref, resource->GetRuntime()->GetEngine()->GetTypeInfoById(typeId));
        }
        if(typeId == asTYPEID_VOID) continue;
        std::string value = GetValueData(ref, typeId);
        if(!replace(fmt, "{}", value)) break;
    }
    std::string* result = new std::string(fmt);
    gen->SetReturnObject(result);
}

static std::string ToString(void* ref, int typeId)
{
    GET_RESOURCE();
    if(typeId & asTYPEID_OBJHANDLE)
    {
        // We're receiving a reference to the handle, so we need to dereference it
        ref = *(void**)ref;
        resource->GetRuntime()->GetEngine()->AddRefScriptObject(ref, resource->GetRuntime()->GetEngine()->GetTypeInfoById(typeId));
    }
    return GetValueData(ref, typeId);
}

static ModuleExtension utilExtension("util", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_GLOBAL_FUNC("uint64 GetTimestamp()", GetTimestamp, "Gets the current timestamp");
    REGISTER_GLOBAL_FUNC("void ShowCallstack(uint maxLevels = 0)", ShowCallstack, "Prints the current callstack for debugging");
    REGISTER_GLOBAL_FUNC("bool Eval(const string&in code)", Eval, "Evals the given code");

    REGISTER_VARIADIC_FUNC("string", "Format", "const string&in fmtText", 32, Format, "Formats a string");
    REGISTER_GLOBAL_FUNC("string ToString(?&in value)", ToString, "Returns string representation of any value");
});
