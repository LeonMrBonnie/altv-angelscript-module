#include "Log.h"
#include "../helpers/module.h"
#include "../runtime.h"
#include "../helpers/convert.h"
#include "../helpers/angelscript.h"
#include "../helpers/benchmark.h"
#include <map>

using namespace Helpers;

#ifdef CLIENT_MODULE
/*
static inline const char* GetNativeTypeName(alt::INative::Type type, bool asRef = false)
{
    switch(type)
    {
        case alt::INative::Type::ARG_BOOL:
        case alt::INative::Type::ARG_BOOL_PTR:
        {
            return "bool";
        }
        case alt::INative::Type::ARG_INT32:
        case alt::INative::Type::ARG_INT32_PTR:
        {
            return "int";
        }
        case alt::INative::Type::ARG_UINT32:
        case alt::INative::Type::ARG_UINT32_PTR:
        {
            return "uint";
        }
        case alt::INative::Type::ARG_FLOAT:
        case alt::INative::Type::ARG_FLOAT_PTR:
        {
            return "float";
        }
        case alt::INative::Type::ARG_VECTOR3:
        case alt::INative::Type::ARG_VECTOR3_PTR:
        {
            if(!asRef) return "Vector3";
            else
                return "Vector3&in";
        }
        case alt::INative::Type::ARG_STRING:
        {
            if(!asRef) return "string";
            else
                return "string&in";
        }
        case alt::INative::Type::ARG_STRUCT:
        {
            if(!asRef) return "MemoryBuffer";
            else
                return "MemoryBuffer&in";
        }
        case alt::INative::Type::ARG_VOID:
        {
            return "void";
        }
    }

    Log::Error << "THIS SHOULD NEVER HAPPEN! FAILED TO CONVERT NATIVE TYPE NAME" << Log::Endl;
    return "";
}
static inline const char* GetNativeDeclaration(alt::INative* native)
{
    std::stringstream stream;
    stream << GetNativeTypeName(native->GetRetnType()) << " " << native->GetName().CStr() << "(";
    auto args     = native->GetArgTypes();
    auto argsSize = args.GetSize();
    for(alt::Size i = 0; i < argsSize; i++)
    {
        stream << GetNativeTypeName(args[i], true);
        if(i != argsSize - 1) stream << ", ";
    }
    stream << ")";
    return stream.str().c_str();
}
*/
static inline int GetNativeTypeId(alt::INative::Type type)
{
    switch(type)
    {
        case alt::INative::Type::ARG_BOOL:
        case alt::INative::Type::ARG_BOOL_PTR:
        {
            return asTYPEID_BOOL;
        }
        case alt::INative::Type::ARG_INT32:
        case alt::INative::Type::ARG_INT32_PTR:
        {
            return asTYPEID_INT32;
        }
        case alt::INative::Type::ARG_UINT32:
        case alt::INative::Type::ARG_UINT32_PTR:
        {
            return asTYPEID_UINT32;
        }
        case alt::INative::Type::ARG_FLOAT:
        case alt::INative::Type::ARG_FLOAT_PTR:
        {
            return asTYPEID_FLOAT;
        }
        case alt::INative::Type::ARG_VECTOR3:
        case alt::INative::Type::ARG_VECTOR3_PTR:
        {
            return AngelScriptRuntime::Instance().GetVector3TypeId();
        }
        case alt::INative::Type::ARG_STRING:
        {
            return AngelScriptRuntime::Instance().GetStringTypeId();
        }
        case alt::INative::Type::ARG_STRUCT:
        {
            return -1;
        }
        case alt::INative::Type::ARG_VOID:
        {
            return asTYPEID_VOID;
        }
    }

    Log::Error << "THIS SHOULD NEVER HAPPEN! FAILED TO CONVERT NATIVE TYPE ID" << Log::Endl;
    return -1;
}

static std::unordered_map<std::string, alt::INative*> nativeMap;

static void InvokeNative(asIScriptGeneric* gen)
{
    static auto ctx = alt::ICore::Instance().CreateNativesContext();
    GET_RESOURCE();
    auto nativeName = *static_cast<std::string*>(gen->GetArgAddress(0));
    AS_ASSERT(nativeMap.count(nativeName) != 0, "Invalid native specified", );
    auto native = nativeMap.at(nativeName);
    AS_ASSERT(native->IsValid(), "Invalid native specified", );

    ctx->Reset();

    auto args     = native->GetArgTypes();
    auto argsSize = args.GetSize();

    // todo: properly implement natives
}

static ModuleExtension nativesExtension("natives", [](asIScriptEngine* engine, DocsGenerator* docs) {
    // REGISTER_VARIADIC_FUNC("?&in", "Invoke", "const string&in native", 16, InvokeNative, "Invokes the specified native");

    auto natives = alt::ICore::Instance().GetAllNatives();
    for(auto native : natives)
    {
        nativeMap.insert({ native->GetName().ToString(), native });
    }
});
#endif
