#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "./module.h"
#include "runtime.h"
#include "bindings/data/vector3.h"
#include "bindings/data/vector2.h"
#include "bindings/data/rgba.h"

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
                THROW_ERROR("The specified output value for the stream synced meta data does not have the correct type");
                return;
            }
            int size = engine->GetSizeOfPrimitiveType(destType);
            memcpy(dest, src, size);
        }
    }

    static std::string GetVarData(asIScriptContext* context, int stackLevel, int varIdx)
    {
        auto& runtime = AngelScriptRuntime::Instance();
        const char* name = context->GetVarName(varIdx, stackLevel);
        void* val = context->GetAddressOfVar(varIdx, stackLevel);
        int valTypeId = context->GetVarTypeId(varIdx, stackLevel);
        std::string valString;
        switch(valTypeId)
        {
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
                /*else if(valTypeId == runtime.GetBaseObjectTypeId()  || 
                        valTypeId == runtime.GetWorldObjectTypeId() || 
                        valTypeId == runtime.GetEntityTypeId()      || 
                        valTypeId == runtime.GetPlayerTypeId()      || 
                        valTypeId == runtime.GetVehicleTypeId())
                {
                    auto obj = static_cast<alt::IBaseObject*>(val);
                    std::stringstream stream;
                    stream << "BaseObject{ type: " << (uint16_t)obj->GetType() << " }";
                    valString = stream.str();
                }*/
                else
                {
                    valString = "<N/A>";
                }
                break;
            }
        }
        std::stringstream str;
        str << name << ": " << valString;
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
            Log::Colored << "~y~" << scriptSection << " (" << std::to_string(line) << "): " << func->GetDeclaration() << Log::Endl;
            Log::Colored << "Vars:" << Log::Endl;
            for(int n = 0; n < context->GetVarCount(i); n++)
            {
                Log::Colored << GetVarData(context, i, n) << Log::Endl;
            }
        }
    }
}
