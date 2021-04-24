#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "./module.h"
#include "runtime.h"
#include "bindings/data/vector3.h"
#include "bindings/data/vector2.h"
#include "bindings/data/rgba.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptarray/scriptarray.h"

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
            if(destType != srcType)
            {
                THROW_ERROR("The specified destination value does not have the correct type");
                return;
            }
            int size = engine->GetSizeOfPrimitiveType(destType);
            memcpy(dest, src, size);
        }
    }

    static std::string GetTypeName(int typeId)
    {
        auto& runtime = AngelScriptRuntime::Instance();
        asITypeInfo* typeInfo = runtime.GetEngine()->GetTypeInfoById(typeId);
        if(typeInfo != nullptr) return typeInfo->GetName();
        else switch(typeId)
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
        auto& runtime = AngelScriptRuntime::Instance();
        const char* name = context->GetVarName(varIdx, stackLevel);
        void* val = context->GetAddressOfVar(varIdx, stackLevel);
        int valTypeId = context->GetVarTypeId(varIdx, stackLevel);
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
                    auto vector = *static_cast<Vector3*>(val);
                    std::stringstream stream;
                    stream << "Vector3{ x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetVector2TypeId())
                {
                    auto vector = *static_cast<Vector2*>(val);
                    std::stringstream stream;
                    stream << "Vector2{ x: " << vector.x << ", y: " << vector.y << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetRGBATypeId())
                {
                    auto rgba = *static_cast<alt::RGBA*>(val);
                    std::stringstream stream;
                    stream << "RGBA{ r: " << rgba.r << ", g: " << rgba.g << ", b: " << rgba.b << ", a: " << rgba.a << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetBaseObjectTypeId()  || 
                        valTypeId == runtime.GetWorldObjectTypeId() || 
                        valTypeId == runtime.GetEntityTypeId()      || 
                        valTypeId == runtime.GetPlayerTypeId()      || 
                        valTypeId == runtime.GetVehicleTypeId())
                {
                    auto obj = static_cast<alt::IBaseObject*>(val);
                    std::stringstream stream;
                    stream << "BaseObject{ type: " << (uint16_t)obj->GetType() << " }";
                    valString = stream.str();
                }
                else if(valTypeId == runtime.GetDictTypeId())
                {
                    auto dict = static_cast<CScriptDictionary*>(val);
                    std::stringstream stream;
                    stream << "Dictionary{ size: " << dict->GetSize() << " }";
                    valString = stream.str();
                }
                else if(valTypeName == "array")
                {
                    auto array = static_cast<CScriptArray*>(val);
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
            const char* scriptSection;
            int line, column;
            func = context->GetFunction(i);
            line = context->GetLineNumber(i, &column, &scriptSection);
            Log::Colored << "~y~" << scriptSection << " (" << std::to_string(line) << ") : " << func->GetDeclaration() << Log::Endl;
            Log::Colored << "~b~Vars:" << Log::Endl;
            for(int n = 0; n < context->GetVarCount(i); n++)
            {
                Log::Colored << "~b~-~w~ " << GetVarData(context, i, n) << Log::Endl;
            }
        }
    }

    static void RegisterArrayTemplateSpecializations(asIScriptEngine* engine)
    {
        // Int
        engine->RegisterObjectType("array<int8>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<int16>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<int>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<int64>", 0, asOBJ_REF);
        // UInt
        engine->RegisterObjectType("array<uint8>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<uint16>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<uint>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<uint64>", 0, asOBJ_REF);
        // String
        //engine->RegisterObjectType("array<string>", 0, asOBJ_REF);
        // Bool
        engine->RegisterObjectType("array<bool>", 0, asOBJ_REF);
        // Floating point
        engine->RegisterObjectType("array<float>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<double>", 0, asOBJ_REF);
        // alt:V Entities
        engine->RegisterObjectType("array<Player@>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<Vehicle@>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<Entity@>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<VoiceChannel@>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<ColShape@>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<Checkpoint@>", 0, asOBJ_REF);
        engine->RegisterObjectType("array<Blip@>", 0, asOBJ_REF);
    }
}
