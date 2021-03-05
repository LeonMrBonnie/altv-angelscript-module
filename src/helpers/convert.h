#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../bindings/data/vector3.h"
#include "../bindings/data/vector2.h"
#include "../runtime.h"

namespace Helpers
{
    // first = typeId, second = value
    static std::tuple<int, void*> MValueToValue(AngelScriptRuntime* runtime, alt::MValueConst& val)
    {
        int type;
        void* valuePtr;
        switch(val->GetType())
        {
            case alt::IMValue::Type::BOOL: 
            {
                type = asTYPEID_BOOL;
                auto value = new bool(val.As<alt::IMValueBool>()->Value());
                valuePtr = value;
                break;
            }
            case alt::IMValue::Type::INT:
            {
                type = asTYPEID_INT64;
                auto value = new int64_t(val.As<alt::IMValueInt>()->Value());
                valuePtr = value;
                break;
            }
            case alt::IMValue::Type::UINT:
            {
                type = asTYPEID_UINT64;
                auto value = new uint64_t(val.As<alt::IMValueUInt>()->Value());
                valuePtr = value;
                break;
            }
            case alt::IMValue::Type::DOUBLE:
            {
                type = asTYPEID_DOUBLE;
                auto value = new double(val.As<alt::IMValueDouble>()->Value());
                valuePtr = value;
                break;
            }
            case alt::IMValue::Type::STRING:
            {
                type = runtime->GetStringTypeId();
                auto str = new std::string(val.As<alt::IMValueString>()->Value().ToString());
                valuePtr = str;
                break;
            }
            case alt::IMValue::Type::LIST:
            {
                // todo: add handle for array in custom events
                /*typeInfo = engine->GetTypeInfoByName("array");
                auto value = val.As<alt::IMValueList>()->Get();
                valuePtr = &value;*/
                break;
            }
            case alt::IMValue::Type::DICT:
            {
                // todo: add handle for dict in custom events
                /*typeInfo = engine->GetTypeInfoByName("dict");
                auto value = val.As<alt::IMValueUInt>()->Value();
                valuePtr = &value;
                break;*/
            }
            case alt::IMValue::Type::BASE_OBJECT:
            {
                type = runtime->GetBaseObjectTypeId();
                auto value = val.As<alt::IMValueBaseObject>()->Value();
                valuePtr = value.Get();
                break;
            }
            case alt::IMValue::Type::VECTOR3:
            {
                type = runtime->GetVector3TypeId();
                auto value = val.As<alt::IMValueVector3>()->Value();
                valuePtr = new Vector3(value[0], value[1], value[2]);
                break;
            }
            case alt::IMValue::Type::VECTOR2:
            {
                type = runtime->GetVector2TypeId();
                auto value = val.As<alt::IMValueVector2>()->Value();
                valuePtr = new Vector2(value[0], value[1]);
                break;
            }
            case alt::IMValue::Type::RGBA:
            {
                type = runtime->GetRGBATypeId();
                auto value = val.As<alt::IMValueRGBA>()->Value();
                valuePtr = new alt::RGBA(value.r, value.g, value.b, value.a);
                break;
            }
        }
        return {type, valuePtr};
    }
    static alt::MValue ValueToMValue(int type, void* value)
    {
        auto& runtime = AngelScriptRuntime::Instance();
        auto& core = alt::ICore::Instance();

        switch(type)
        {
            // Bool
            case asTYPEID_BOOL: return core.CreateMValueBool(*static_cast<bool*>(value));
            // Int
            case asTYPEID_INT8: return core.CreateMValueInt(*static_cast<int8_t*>(value));
            case asTYPEID_INT16: return core.CreateMValueInt(*static_cast<int16_t*>(value));
            case asTYPEID_INT32: return core.CreateMValueInt(*static_cast<int32_t*>(value));
            case asTYPEID_INT64: return core.CreateMValueInt(*static_cast<int64_t*>(value));
            // Uint
            case asTYPEID_UINT8: return core.CreateMValueUInt(*static_cast<uint8_t*>(value));
            case asTYPEID_UINT16: return core.CreateMValueUInt(*static_cast<uint16_t*>(value));
            case asTYPEID_UINT32: return core.CreateMValueUInt(*static_cast<uint32_t*>(value));
            case asTYPEID_UINT64: return core.CreateMValueUInt(*static_cast<uint64_t*>(value));
            // Float
            case asTYPEID_FLOAT: return core.CreateMValueDouble(*static_cast<float*>(value));
            case asTYPEID_DOUBLE: return core.CreateMValueDouble(*static_cast<double*>(value));
            // Not primitive
            default:
            {
                // Because these type ids are not constant and can change with every update, we have to check
                // them in an if chain here, instead of directly in the switch expression
                if(type == runtime.GetStringTypeId()) 
                    return core.CreateMValueString(*static_cast<std::string*>(value));
                else if(type == runtime.GetVector3TypeId())
                {
                    auto vector = *static_cast<Vector3*>(value);
                    return core.CreateMValueVector3({vector.x, vector.y, vector.z});
                }
                else if(type == runtime.GetVector2TypeId())
                {
                    auto vector = *static_cast<Vector2*>(value);
                    return core.CreateMValueVector2({vector.x, vector.y});
                }
                else if(type == runtime.GetRGBATypeId())
                {
                    auto rgba = *static_cast<alt::RGBA*>(value);
                    return core.CreateMValueRGBA(rgba);
                }
                else if(type == runtime.GetBaseObjectTypeId()  || 
                        type == runtime.GetWorldObjectTypeId() || 
                        type == runtime.GetEntityTypeId()      || 
                        type == runtime.GetPlayerTypeId()      || 
                        type == runtime.GetVehicleTypeId())
                    return core.CreateMValueBaseObject(static_cast<alt::IBaseObject*>(value));

                break;
            }
        }
        
        // If the specified type is unknown, just return a nil value
        Log::Warning << "Invalid value type for MValue conversion: " << runtime.GetEngine()->GetTypeInfoById(type)->GetName() << " (" << type << ")" << Log::Endl;
        return core.CreateMValueNil();
    }
    static bool IsTypePrimitive(int type)
    {
        switch(type)
        {
            // Bool
            case asTYPEID_BOOL: 
            // Int
            case asTYPEID_INT8:
            case asTYPEID_INT16:
            case asTYPEID_INT32:
            case asTYPEID_INT64:
            // Uint
            case asTYPEID_UINT8:
            case asTYPEID_UINT16:
            case asTYPEID_UINT32:
            case asTYPEID_UINT64:
            // Float
            case asTYPEID_FLOAT:
            case asTYPEID_DOUBLE:
                return true;
            default: 
                return false;
        }
    }

    class MValueFunc : public alt::IMValueFunction::Impl
    {
        AngelScriptResource* resource;
        asIScriptFunction* func;

    public:
        MValueFunc(AngelScriptResource* resource, asIScriptFunction* func) : resource(resource), func(func) {}

        alt::MValue Call(alt::MValueArgs args) const override
        {
            auto context = resource->GetContext();
            context->Prepare(func);
            for(uint32_t i = 0; i < args.GetSize(); i++)
            {
                int ret;
                auto arg = args[i];
                auto [type, val] = MValueToValue(resource->GetRuntime(), arg);
                if(IsTypePrimitive(type)) ret = context->SetArgAddress(i, val);
                else ret = context->SetArgObject(i, val);
                CHECK_AS_RETURN("Call MValue function", ret, nullptr);
            }
            context->Execute();
            auto value = ValueToMValue(func->GetReturnTypeId(), context->GetReturnAddress());
            context->Unprepare();
            return value;
        }
    };
}