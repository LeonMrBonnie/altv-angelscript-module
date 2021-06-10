#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "./module.h"
#include "runtime.h"
#include "bindings/data/vector3.h"
#include "bindings/data/vector2.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptarray/scriptarray.h"

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
        auto&        runtime  = AngelScriptRuntime::Instance();
        asITypeInfo* typeInfo = runtime.GetEngine()->GetTypeInfoById(typeId);
        if(typeInfo != nullptr) return typeInfo->GetName();
        else
            switch(typeId)
            {
                case asTYPEID_VOID: return "void";
                case asTYPEID_BOOL: return "bool";
                case asTYPEID_INT8: return "int8";
                case asTYPEID_INT16: return "int16";
                case asTYPEID_INT32: return "int32";
                case asTYPEID_INT64: return "int64";
                case asTYPEID_UINT8: return "uint8";
                case asTYPEID_UINT16: return "uint16";
                case asTYPEID_UINT32: return "uint32";
                case asTYPEID_UINT64: return "uint64";
                case asTYPEID_FLOAT: return "float";
                case asTYPEID_DOUBLE: return "double";
            }
        return "unknown";
    }
    static std::string GetVarData(asIScriptContext* context, int stackLevel, int varIdx)
    {
        auto&       runtime     = AngelScriptRuntime::Instance();
        const char* name        = context->GetVarName(varIdx, stackLevel);
        void*       val         = context->GetAddressOfVar(varIdx, stackLevel);
        int         valTypeId   = context->GetVarTypeId(varIdx, stackLevel);
        std::string valTypeName = GetTypeName(valTypeId);
        std::string valString;
        if(val == nullptr) valTypeId = asTYPEID_VOID;

        switch(valTypeId)
        {
            // Unknown / Void
            case asTYPEID_VOID:
            {
                valString = "<void>";
                break;
            }
            // Bool
            case asTYPEID_BOOL:
            {
                valString = std::to_string(*(bool*)val);
                break;
            }
            // Int
            case asTYPEID_INT8:
            {
                valString = std::to_string(*(int8_t*)val);
                break;
            }
            case asTYPEID_INT16:
            {
                valString = std::to_string(*(int16_t*)val);
                break;
            }
            case asTYPEID_INT32:
            {
                valString = std::to_string(*(int32_t*)val);
                break;
            }
            case asTYPEID_INT64:
            {
                valString = std::to_string(*(int64_t*)val);
                break;
            }
            // Uint
            case asTYPEID_UINT8:
            {
                valString = std::to_string(*(uint8_t*)val);
                break;
            }
            case asTYPEID_UINT16:
            {
                valString = std::to_string(*(uint16_t*)val);
                break;
            }
            case asTYPEID_UINT32:
            {
                valString = std::to_string(*(uint32_t*)val);
                break;
            }
            case asTYPEID_UINT64:
            {
                valString = std::to_string(*(uint64_t*)val);
                break;
            }
            // Float
            case asTYPEID_FLOAT:
            {
                valString = std::to_string(*(float*)val);
                break;
            }
            case asTYPEID_DOUBLE:
            {
                valString = std::to_string(*(double*)val);
                break;
            }
            // Not primitive
            default:
            {
                // Because these type ids are not constant and can change with every update, we have to check
                // them in an if chain here, instead of directly in the switch expression
                if(valTypeId == runtime.GetStringTypeId())
                {
                    valString = *(std::string*)val;
                }
                else if(valTypeId == runtime.GetVector3TypeId())
                {
                    auto              vector = *static_cast<Data::Vector3*>(val);
                    std::stringstream stream;
                    stream << "Vector3{ x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetVector2TypeId())
                {
                    auto              vector = *static_cast<Data::Vector2*>(val);
                    std::stringstream stream;
                    stream << "Vector2{ x: " << vector.x << ", y: " << vector.y << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetRGBATypeId())
                {
                    auto              rgba = *static_cast<alt::RGBA*>(val);
                    std::stringstream stream;
                    stream << "RGBA{ r: " << rgba.r << ", g: " << rgba.g << ", b: " << rgba.b << ", a: " << rgba.a << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetBaseObjectTypeId() || valTypeId == runtime.GetWorldObjectTypeId() ||
                        valTypeId == runtime.GetEntityTypeId() || valTypeId == runtime.GetPlayerTypeId() || valTypeId == runtime.GetVehicleTypeId())
                {
                    auto              obj = static_cast<alt::IBaseObject*>(val);
                    std::stringstream stream;
                    stream << "BaseObject{ type: " << (uint16_t)obj->GetType() << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetDictTypeId())
                {
                    auto              dict = static_cast<CScriptDictionary*>(val);
                    std::stringstream stream;
                    stream << "Dictionary{ size: " << dict->GetSize() << " }";
                    valString = stream.str();
                }
                else if(valTypeName == "array")
                {
                    auto              array = static_cast<CScriptArray*>(val);
                    std::stringstream stream;
                    stream << "Array{ size: " << array->GetSize() << ", type: " << GetTypeName(array->GetElementTypeId()) << " }";
                    valString = stream.str();
                }
                else
                {
                    valString = "<N/A>";
                }
                break;
            }
        }

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
                Log::Colored << "~b~-~w~ " << GetVarData(context, i, n) << Log::Endl;
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
        return 0;
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
}  // namespace Helpers
