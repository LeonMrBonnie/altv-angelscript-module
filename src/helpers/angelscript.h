#pragma once

#include <regex>
#include <map>

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "./module.h"
#include "./libImport.h"
#include "runtime.h"
#include "bindings/data/vector3.h"
#include "bindings/data/vector2.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/include/angelscript.h"

#define CHECK_FUNCTION_RETURN(r, ret)                                                           \
    if(r == asEXECUTION_EXCEPTION)                                                              \
    {                                                                                           \
        Log::Error << "An exception occured while executing the script function." << Log::Endl; \
        Helpers::PrintException(context);                                                       \
        context->Unprepare();                                                                   \
        return ret;                                                                             \
    }

#define AS_ASSERT(condition, error, result) \
    if(!(condition))                        \
    {                                       \
        THROW_ERROR(error);                 \
        return result;                      \
    }

static std::unordered_map<int, std::string> typeNameMap = {
    { asTYPEID_VOID, "void" },     { asTYPEID_BOOL, "bool" },     { asTYPEID_INT8, "int8" },   { asTYPEID_INT16, "int16" },
    { asTYPEID_INT32, "int32" },   { asTYPEID_INT64, "int64" },   { asTYPEID_UINT8, "uint8" }, { asTYPEID_UINT16, "uint16" },
    { asTYPEID_UINT32, "uint32" }, { asTYPEID_UINT64, "uint64" }, { asTYPEID_FLOAT, "float" }, { asTYPEID_DOUBLE, "double" }
};

namespace Helpers
{
    static void CopyAngelscriptValue(asIScriptEngine* engine, void* src, int srcType, void* dest, int destType)
    {
        if(destType & asTYPEID_OBJHANDLE && srcType & asTYPEID_MASK_OBJECT)
        {
            // RefCastObject will increment the refCount of the returned pointer if successful
            engine->RefCastObject(src, engine->GetTypeInfoById(srcType), engine->GetTypeInfoById(destType), reinterpret_cast<void**>(dest));
        }
        else if(destType & asTYPEID_MASK_OBJECT && srcType == destType)
        {
            engine->AssignScriptObject(dest, src, engine->GetTypeInfoById(srcType));
        }
        else
        {
            AS_ASSERT(destType == srcType, "Specified destination value does not have the correct type", );

            int size = engine->GetSizeOfPrimitiveType(destType);
            memcpy(dest, src, size);
        }
    }

    static std::string GetTypeName(int typeId)
    {
        if(typeNameMap.count(typeId) != 0) return typeNameMap.at(typeId);
        auto&        runtime  = AngelScriptRuntime::Instance();
        asITypeInfo* typeInfo = runtime.GetEngine()->GetTypeInfoById(typeId);
        if(typeInfo != nullptr) return typeInfo->GetName();
        return "unknown";
    }
    // Returns -1 if typeId was not found
    static int GetTypeIdFromName(std::string& name)
    {
        auto typeInfo = AngelScriptRuntime::Instance().GetEngine()->GetTypeInfoByName(name.c_str());
        if(typeInfo) return typeInfo->GetTypeId();
        for(auto pair : typeNameMap)
        {
            if(pair.second == name) return pair.first;
        }
        return -1;
    }
    static uint32_t GetTypeSize(int typeId)
    {
        auto engine   = AngelScriptRuntime::Instance().GetEngine();
        auto typeInfo = engine->GetTypeInfoById(typeId);
        if(typeInfo) return typeInfo->GetSize();
        return engine->GetSizeOfPrimitiveType(typeId);
    }
    static std::string GetValueData(void* val, int typeId)
    {
        if(val == nullptr) typeId = asTYPEID_VOID;
        AngelScriptRuntime& runtime = AngelScriptRuntime::Instance();
        std::string         result;
        switch(typeId)
        {
            // Unknown / Void
            case asTYPEID_VOID:
            {
                result = "<void>";
                break;
            }
            // Bool
            case asTYPEID_BOOL:
            {
                result = std::to_string(*(bool*)val);
                break;
            }
            // Int
            case asTYPEID_INT8:
            {
                result = std::to_string(*(int8_t*)val);
                break;
            }
            case asTYPEID_INT16:
            {
                result = std::to_string(*(int16_t*)val);
                break;
            }
            case asTYPEID_INT32:
            {
                result = std::to_string(*(int32_t*)val);
                break;
            }
            case asTYPEID_INT64:
            {
                result = std::to_string(*(int64_t*)val);
                break;
            }
            // Uint
            case asTYPEID_UINT8:
            {
                result = std::to_string(*(uint8_t*)val);
                break;
            }
            case asTYPEID_UINT16:
            {
                result = std::to_string(*(uint16_t*)val);
                break;
            }
            case asTYPEID_UINT32:
            {
                result = std::to_string(*(uint32_t*)val);
                break;
            }
            case asTYPEID_UINT64:
            {
                result = std::to_string(*(uint64_t*)val);
                break;
            }
            // Float
            case asTYPEID_FLOAT:
            {
                result = std::to_string(*(float*)val);
                break;
            }
            case asTYPEID_DOUBLE:
            {
                result = std::to_string(*(double*)val);
                break;
            }
            // Not primitive
            default:
            {
                // Because these type ids are not constant and can change with every update, we have to check
                // them in an if chain here, instead of directly in the switch expression
                if(typeId == runtime.GetStringTypeId())
                {
                    result = *(std::string*)val;
                }
                else if(typeId == runtime.GetVector3TypeId())
                {
                    auto              vector = *static_cast<Data::Vector3*>(val);
                    std::stringstream stream;
                    stream << "Vector3{ x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << " }";
                    result = stream.str();
                }
                else if(typeId == runtime.GetVector2TypeId())
                {
                    auto              vector = *static_cast<Data::Vector2*>(val);
                    std::stringstream stream;
                    stream << "Vector2{ x: " << vector.x << ", y: " << vector.y << " }";
                    result = stream.str();
                }
                else if(typeId == runtime.GetRGBATypeId())
                {
                    auto              rgba = *static_cast<alt::RGBA*>(val);
                    std::stringstream stream;
                    stream << "RGBA{ r: " << rgba.r << ", g: " << rgba.g << ", b: " << rgba.b << ", a: " << rgba.a << " }";
                    result = stream.str();
                }
                else if(typeId == runtime.GetBaseObjectTypeId() || typeId == runtime.GetWorldObjectTypeId() || typeId == runtime.GetEntityTypeId() ||
                        typeId == runtime.GetPlayerTypeId() || typeId == runtime.GetVehicleTypeId())
                {
                    auto              obj = static_cast<alt::IBaseObject*>(val);
                    std::stringstream stream;
                    stream << "BaseObject{ type: " << (uint16_t)obj->GetType() << " }";
                    result = stream.str();
                }
                else if(typeId == runtime.GetDictTypeId())
                {
                    auto              dict = static_cast<CScriptDictionary*>(val);
                    std::stringstream stream;
                    stream << "Dictionary{ size: " << dict->GetSize() << " }";
                    result = stream.str();
                }
                else if(GetTypeName(typeId) == "array")
                {
                    auto              array = static_cast<CScriptArray*>(val);
                    std::stringstream stream;
                    stream << "Array{ size: " << array->GetSize() << ", type: " << GetTypeName(array->GetElementTypeId()) << " }";
                    result = stream.str();
                }
                else
                {
                    result = "<N/A>";
                }
                break;
            }
        }
        return result;
    }

    static inline std::string GetCallstackVarData(asIScriptContext* context, int stackLevel, int varIdx)
    {
        auto&       runtime     = AngelScriptRuntime::Instance();
        const char* name        = context->GetVarName(varIdx, stackLevel);
        void*       val         = context->GetAddressOfVar(varIdx, stackLevel);
        int         valTypeId   = context->GetVarTypeId(varIdx, stackLevel);
        std::string valTypeName = GetTypeName(valTypeId);
        std::string valString   = GetValueData(val, valTypeId);

        std::stringstream str;
        str << "[~y~" << valTypeName << "~w~] " << name << ": ~b~" << valString;
        return str.str();
    }
    static void PrintCallstack(asIScriptContext* context, uint32_t maxLevels)
    {
        uint32_t size = context->GetCallstackSize();
        if(maxLevels != 0 && maxLevels < size) size = maxLevels;
        Log::Colored << "~b~Callstack: (" << std::to_string(size) << " levels)" << Log::Endl;
        for(uint32_t i = 0; i < size; i++)
        {
            Log::Colored << "~y~--------------------" << Log::Endl;
            asIScriptFunction* func = nullptr;
            const char*        scriptSection;
            int                line, column;
            func = context->GetFunction(i);
            line = context->GetLineNumber(i, &column, &scriptSection);
            if(!func) continue;
            if(func->GetFuncType() != asFUNC_SCRIPT) continue;
            Log::Colored << "~y~" << scriptSection << " (" << std::to_string(line) << ") : " << func->GetDeclaration() << Log::Endl;
            Log::Colored << "~b~Vars:" << Log::Endl;
            for(int n = 0; n < context->GetVarCount(i); n++)
            {
                Log::Colored << "~b~-~w~ " << GetCallstackVarData(context, i, n) << Log::Endl;
            }
        }
    }
    static void PrintException(asIScriptContext* context)
    {
        if(context->GetState() != asEXECUTION_EXCEPTION) return;
        auto func      = context->GetExceptionFunction();
        auto line      = context->GetExceptionLineNumber();
        auto exception = context->GetExceptionString();
        Log::Error << func->GetScriptSectionName() << " (" << line << "): " << exception << Log::Endl;
    }

    // Handles includes
    static int IncludeHandler(const char* include, const char* from, CScriptBuilder* builder, void* data)
    {
        // todo: add support for relative paths
        auto resource = static_cast<AngelScriptResource*>(data);
        auto src      = resource->ReadFile(include);
        int  r        = builder->AddSectionFromMemory(include, src.CStr(), (asUINT)src.GetSize());
        CHECK_AS_RETURN("Include", r, -1);
        return 0;
    }

    // Handles pragma directives
    static int PragmaHandler(const std::string& pragmaText, CScriptBuilder& builder, void* data)
    {
        auto resource = static_cast<AngelScriptResource*>(data);
        if(LibraryImport::LibraryImportPragmaHandler(pragmaText, resource)) return 0;

        Log::Warning << "Unknown pragma directive used: '" << pragmaText << "'" << Log::Endl;
        return -1;
    }

    // Handles infos, warnings, errors etc. by AngelScript
    static void MessageHandler(const asSMessageInfo* msg, void* param)
    {
        std::stringstream stream;
        stream << msg->section << " (" << std::to_string(msg->row) << "): " << msg->message;
        switch(msg->type)
        {
            case asMSGTYPE_INFORMATION:
            {
                Log::Info << stream.str() << Log::Endl;
                break;
            }
            case asMSGTYPE_WARNING:
            {
                Log::Warning << stream.str() << Log::Endl;
                break;
            }
            case asMSGTYPE_ERROR:
            {
                Log::Error << stream.str() << Log::Endl;
                break;
            }
        }
    }

    static void CircularRefDetectedHandler(asITypeInfo* type, const void*, void*)
    {
        auto resource = AngelScriptRuntime::Instance().GetResourceByModule(type->GetModule());
        Log::Warning << "!!! Circular ref detected !!!" << Log::Endl;
        Log::Warning << "Type: " << type->GetName() << Log::Endl;
        Log::Warning << "Resource: " << resource->GetIResource()->GetName() << Log::Endl;
    }

    static const char* GetContextStateName(asEContextState state)
    {
        switch(state)
        {
            case asEXECUTION_FINISHED: return "FINISHED";
            case asEXECUTION_SUSPENDED: return "SUSPENDED";
            case asEXECUTION_ABORTED: return "ABORTED";
            case asEXECUTION_EXCEPTION: return "EXCEPTION";
            case asEXECUTION_PREPARED: return "PREPARED";
            case asEXECUTION_UNINITIALIZED: return "UNINITIALIZED";
            case asEXECUTION_ACTIVE: return "ACTIVE";
            case asEXECUTION_ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }

    // first = amount of total methods, second = amount of total properties
    static std::pair<int, int> GetRegisteredClassesStats(asIScriptEngine* engine)
    {
        int totalMethods    = 0;
        int totalProperties = 0;
        for(asUINT i = 0; i < engine->GetObjectTypeCount(); i++)
        {
            auto typeClass = engine->GetObjectTypeByIndex(i);
            totalMethods += typeClass->GetMethodCount();
            totalProperties += typeClass->GetPropertyCount();
        }
        return { totalMethods, totalProperties };
    }

    struct FunctionInfo
    {
        bool                     valid = false;
        std::string              returnTypeName;
        std::string              functionName;
        std::vector<std::string> argTypes;
    };
    static FunctionInfo GetFunctionInfoFromDecl(const std::string& decl)
    {
        static std::regex funcInfoRegex("(.*?) (.*?)\\((.*?)\\)");

        FunctionInfo info;
        std::smatch  results;
        auto         result = std::regex_search(decl.cbegin(), decl.cend(), results, funcInfoRegex);
        if(!result) return info;

        info.valid          = true;
        info.returnTypeName = results[1].str();
        info.functionName   = results[2].str();
        std::vector<std::string> argTypes;
        for(int i = 3; i < results.size(); i++)
        {
            argTypes.push_back(results[i].str());
        }
        return info;
    }

#ifdef DEBUG_MEMORY
    static std::unordered_map<void*, size_t> allocations = {};
    extern bool                              showAllocationMessages;

    static inline std::string PointerToHex(void* ptr)
    {
        std::stringstream stream;
        stream << "0x" << std::hex << (uintptr_t)ptr;
        return stream.str();
    }
#endif

    static void* MemoryAlloc(size_t size)
    {
        void* ptr = std::malloc(size);
#ifdef DEBUG_MEMORY
        if(showAllocationMessages)
            Log::Colored << "~lc~[DEBUG] "
                         << "~ly~Allocated ~lk~" << size << " ~lc~bytes (Ptr: ~lk~" << PointerToHex(ptr) << "~lc~)" << Log::Endl;
        allocations.insert({ ptr, size });
#endif
        return ptr;
    }
    static void MemoryFree(void* ptr)
    {
#ifdef DEBUG_MEMORY
        if(showAllocationMessages)
        {
            auto size = allocations.at(ptr);
            Log::Colored << "~lc~[DEBUG] "
                         << "~lg~Freed ~lk~" << size << " ~lc~bytes (Ptr: ~lk~" << PointerToHex(ptr) << "~lc~)" << Log::Endl;
        }
        allocations.erase(ptr);
#endif
        std::free(ptr);
    }
}  // namespace Helpers
