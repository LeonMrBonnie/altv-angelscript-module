#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../bindings/vector3.h"

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
    static std::pair<int, void*> GetMValueValue(asIScriptEngine* engine, alt::MValueConst& val)
    {
        asITypeInfo* typeInfo;
        void* valuePtr;
        switch(val->GetType())
        {
            case alt::IMValue::Type::BOOL: 
            {
                typeInfo = engine->GetTypeInfoByName("bool");
                auto value = val.As<alt::IMValueBool>()->Value();
                valuePtr = &value;
                break;
            }
            case alt::IMValue::Type::INT:
            {
                typeInfo = engine->GetTypeInfoByName("int");
                auto value = val.As<alt::IMValueInt>()->Value();
                valuePtr = &value;
                break;
            }
            case alt::IMValue::Type::UINT:
            {
                typeInfo = engine->GetTypeInfoByName("uint");
                auto value = val.As<alt::IMValueUInt>()->Value();
                valuePtr = &value;
                break;
            }
            case alt::IMValue::Type::DOUBLE:
            {
                typeInfo = engine->GetTypeInfoByName("double");
                auto value = val.As<alt::IMValueDouble>()->Value();
                valuePtr = &value;
                break;
            }
            case alt::IMValue::Type::STRING:
            {
                typeInfo = engine->GetTypeInfoByName("string");
                auto value = val.As<alt::IMValueString>()->Value().CStr();
                valuePtr = &value;
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
                typeInfo = engine->GetTypeInfoByName("BaseObject");
                auto value = val.As<alt::IMValueBaseObject>()->Value();
                valuePtr = value.Get();
                break;
            }
            case alt::IMValue::Type::VECTOR3:
            {
                typeInfo = engine->GetTypeInfoByName("Vector3f");
                auto value = val.As<alt::IMValueVector3>()->Value();
                valuePtr = &Vector3<float>(value[0], value[1], value[2]);
                break;
            }
            //case alt::IMValue::Type::VECTOR2: return engine->GetTypeInfoByName("Vector2f");
            //case alt::IMValue::Type::RGBA: return engine->GetTypeInfoByName("RGBA");
        }
        return {typeInfo->GetTypeId(), valuePtr};
    }
    static alt::MValue ValueToMValue(asITypeInfo* type, void* value)
    {
        auto& core = alt::ICore::Instance();
        std::string typeName = type->GetName();

        if(typeName == "bool") return core.CreateMValueBool(*static_cast<bool*>(value));
        else if(typeName == "int"    || typeName == "int64") return core.CreateMValueInt(*static_cast<int64_t*>(value));
        else if(typeName == "uint"   || typeName == "uint64") return core.CreateMValueInt(*static_cast<uint32_t*>(value));
        else if(typeName == "float"  || typeName == "double") return core.CreateMValueDouble(*static_cast<double*>(value));
        else if(typeName == "string") return core.CreateMValueString(*static_cast<std::string*>(value));
        else if(typeName == "BaseObject" || typeName == "WorldObject" || typeName == "Entity" || typeName == "Player" || typeName == "Vehicle") 
            return core.CreateMValueBaseObject(alt::Ref<alt::IBaseObject>(static_cast<alt::IBaseObject*>(value)));
    }
    /*alt::MValue GetMValueFromTypeInfo(asITypeInfo* type)
    {
        // todo: implement this
    }*/
}