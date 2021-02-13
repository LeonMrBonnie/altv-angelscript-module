#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../bindings/vector3.h"
#include "../bindings/vector2.h"
#include "../runtime.h"

namespace Helpers
{
    static asITypeInfo* GetTypeInfoFromMValue(asIScriptEngine* engine, alt::MValueConst& val)
    {
        switch(val->GetType())
        {
            case alt::IMValue::Type::BOOL: return engine->GetTypeInfoByName("bool");
            case alt::IMValue::Type::INT: return engine->GetTypeInfoByName("int");
            case alt::IMValue::Type::UINT: return engine->GetTypeInfoByName("uint");
            case alt::IMValue::Type::DOUBLE: return engine->GetTypeInfoByName("double");
            case alt::IMValue::Type::STRING: return engine->GetTypeInfoByName("string");
            case alt::IMValue::Type::LIST: return engine->GetTypeInfoByName("array");
            case alt::IMValue::Type::DICT: return engine->GetTypeInfoByName("dict");
            case alt::IMValue::Type::BASE_OBJECT: return engine->GetTypeInfoByName("BaseObject");
            case alt::IMValue::Type::VECTOR3: return engine->GetTypeInfoByName("Vector3f");
            //case alt::IMValue::Type::VECTOR2: return engine->GetTypeInfoByName("Vector2f");
            //case alt::IMValue::Type::RGBA: return engine->GetTypeInfoByName("RGBA");
        }
    }
    // first = typeId, second = value
    static std::pair<int, void*> MValueToValue(AngelScriptRuntime* runtime, alt::MValueConst& val)
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
                // any doesn't support uints so we have to store it as an int64 instead
                type = asTYPEID_INT64;
                auto value = new int64_t(val.As<alt::IMValueUInt>()->Value());
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
                type = runtime->GetEngine()->GetTypeInfoByName("Vector3f")->GetTypeId();
                auto value = val.As<alt::IMValueVector3>()->Value();
                valuePtr = new Vector3<float>(value[0], value[1], value[2]);
                break;
            }
            case alt::IMValue::Type::VECTOR2:
            {
                type = runtime->GetEngine()->GetTypeInfoByName("Vector2f")->GetTypeId();
                auto value = val.As<alt::IMValueVector2>()->Value();
                valuePtr = new Vector2<float>(value[0], value[1]);
                break;
            }
            //case alt::IMValue::Type::RGBA: return engine->GetTypeInfoByName("RGBA");
        }
        return {type, valuePtr};
    }
    static alt::MValue ValueToMValue(int type, void* value)
    {
        auto& runtime = AngelScriptRuntime::Instance();
        auto& core = alt::ICore::Instance();

        if(type == asTYPEID_BOOL) return core.CreateMValueBool(*static_cast<bool*>(value));
        else if(type == asTYPEID_INT32    /*|| type == asTYPEID_INT64*/)  return core.CreateMValueInt(*static_cast<int32_t*>(value));
        else if(type == asTYPEID_UINT32   /*|| type == asTYPEID_UINT64*/) return core.CreateMValueUInt(*static_cast<uint32_t*>(value));
        else if(type == asTYPEID_FLOAT    /*|| type == asTYPEID_DOUBLE*/) return core.CreateMValueDouble(*(float*)value);
        else if(type == runtime.GetStringTypeId()) return core.CreateMValueString(*static_cast<std::string*>(value));
        else if(type == runtime.GetBaseObjectTypeId()  || 
                type == runtime.GetWorldObjectTypeId() || 
                type == runtime.GetEntityTypeId()      || 
                type == runtime.GetPlayerTypeId()      || 
                type == runtime.GetVehicleTypeId())
            return core.CreateMValueBaseObject(alt::Ref<alt::IBaseObject>(static_cast<alt::IBaseObject*>(value)));
        
        return core.CreateMValueNil();
    }
    static bool IsTypePrimitive(int type)
    {
        switch(type)
        {
            case asTYPEID_BOOL: 
            case asTYPEID_INT32:
            case asTYPEID_INT64:
            case asTYPEID_UINT32:
            case asTYPEID_UINT64:
            case asTYPEID_FLOAT:
            case asTYPEID_DOUBLE:
                return true;
            default: 
                return false;
        }
    }
}