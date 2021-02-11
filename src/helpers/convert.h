#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"

namespace Helpers
{
    asITypeInfo* GetTypeInfoFromMValue(asIScriptEngine* engine, alt::MValueConst& val)
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
    /*alt::MValue GetMValueFromTypeInfo(asITypeInfo* type)
    {
        // todo: implement this
    }*/
}