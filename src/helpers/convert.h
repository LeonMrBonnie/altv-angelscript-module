#pragma once
#include <random>
#include <stdint.h>

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../bindings/data/vector3.h"
#include "../bindings/data/vector2.h"
#include "../runtime.h"
#include "angelscript/add_on/scriptdictionary/scriptdictionary.h"
#include "angelscript.h"

namespace Helpers
{
    // first = typeId, second = value
    static std::tuple<int, void*> MValueToValue(AngelScriptRuntime* runtime, alt::MValueConst& val)
    {
        int   type;
        void* valuePtr;
        switch(val->GetType())
        {
            case alt::IMValue::Type::NONE:
            case alt::IMValue::Type::NIL:
            {
                type     = asTYPEID_VOID;
                valuePtr = nullptr;
                break;
            }
            case alt::IMValue::Type::BOOL:
            {
                type       = asTYPEID_BOOL;
                auto value = new bool(val.As<alt::IMValueBool>()->Value());
                valuePtr   = value;
                break;
            }
            case alt::IMValue::Type::INT:
            {
                auto mval = val.As<alt::IMValueInt>()->Value();
                if(mval <= INT32_MAX)
                {
                    type     = asTYPEID_INT32;
                    valuePtr = new int32_t(mval);
                }
                else
                {
                    type     = asTYPEID_INT64;
                    valuePtr = new int64_t(mval);
                }
                break;
            }
            case alt::IMValue::Type::UINT:
            {
                auto mval = val.As<alt::IMValueUInt>()->Value();
                if(mval <= UINT32_MAX)
                {
                    type     = asTYPEID_UINT32;
                    valuePtr = new uint32_t(mval);
                }
                else
                {
                    type     = asTYPEID_UINT64;
                    valuePtr = new uint64_t(mval);
                }
                break;
            }
            case alt::IMValue::Type::DOUBLE:
            {
                type       = asTYPEID_DOUBLE;
                auto value = new double(val.As<alt::IMValueDouble>()->Value());
                valuePtr   = value;
                break;
            }
            case alt::IMValue::Type::STRING:
            {
                type     = runtime->GetTypeInfoCache().Get("string")->GetTypeId();
                auto str = new std::string(val.As<alt::IMValueString>()->Value().ToString());
                valuePtr = str;
                break;
            }
            case alt::IMValue::Type::LIST:
            {
                // We pass the list MValue as a dict, because AngelScript doesn't support arrays with dynamic value types
                auto dict = CScriptDictionary::Create(runtime->GetEngine());
                auto list = val.As<alt::IMValueList>();
                for(uint64_t i = 0; i < list->GetSize(); i++)
                {
                    auto value       = list->Get(i);
                    auto [type, val] = MValueToValue(runtime, value);
                    // Set the type to int64 if its an uint, because the dict doesn't support uint
                    if(type == asTYPEID_UINT64) type = asTYPEID_INT64;
                    dict->Set(std::to_string(i), val, type);
                }
                type     = runtime->GetTypeInfoCache().Get("dictionary")->GetTypeId();
                valuePtr = dict;
                break;
            }
            case alt::IMValue::Type::DICT:
            {
                auto dict  = CScriptDictionary::Create(runtime->GetEngine());
                auto value = val.As<alt::IMValueDict>();
                for(auto it = value->Begin(); it; it = value->Next())
                {
                    auto itValue     = it->GetValue();
                    auto [type, val] = MValueToValue(runtime, itValue);
                    // Set the type to int64 if its an uint, because the dict doesn't support uint
                    if(type == asTYPEID_UINT64) type = asTYPEID_INT64;
                    dict->Set(it->GetKey().ToString(), val, type);
                }
                type     = runtime->GetTypeInfoCache().Get("dictionary")->GetTypeId();
                valuePtr = dict;
                break;
            }
            case alt::IMValue::Type::BASE_OBJECT:
            {
                type       = runtime->GetTypeInfoCache().Get("alt::BaseObject@")->GetTypeId();
                auto value = val.As<alt::IMValueBaseObject>()->Value();
                valuePtr   = value.Get();
                break;
            }
            case alt::IMValue::Type::VECTOR3:
            {
                type       = runtime->GetTypeInfoCache().Get("alt::Vector3")->GetTypeId();
                auto value = val.As<alt::IMValueVector3>()->Value();
                valuePtr   = new Data::Vector3(value[0], value[1], value[2]);
                break;
            }
            case alt::IMValue::Type::VECTOR2:
            {
                type       = runtime->GetTypeInfoCache().Get("alt::Vector2")->GetTypeId();
                auto value = val.As<alt::IMValueVector2>()->Value();
                valuePtr   = new Data::Vector2(value[0], value[1]);
                break;
            }
            case alt::IMValue::Type::RGBA:
            {
                type       = runtime->GetTypeInfoCache().Get("alt::RGBA")->GetTypeId();
                auto value = val.As<alt::IMValueRGBA>()->Value();
                valuePtr   = new alt::RGBA(value.r, value.g, value.b, value.a);
                break;
            }
            case alt::IMValue::Type::BYTE_ARRAY:
            {
                type       = runtime->GetTypeInfoCache().Get("array<uint8>")->GetTypeId();
                auto value = val.As<alt::IMValueByteArray>()->GetData();
                valuePtr   = runtime->CreateByteArray(const_cast<uint8_t*>(value));
                break;
            }
        }
        return { type, valuePtr };
    }
    static alt::MValue ValueToMValue(int type, void* value)
    {
        auto& runtime = AngelScriptRuntime::Instance();
        auto& core    = alt::ICore::Instance();

        switch(type)
        {
            case asTYPEID_VOID: return core.CreateMValueNil();
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
                if(type == runtime.GetTypeInfoCache().Get("string")->GetTypeId()) return core.CreateMValueString(*static_cast<std::string*>(value));
                else if(type == runtime.GetTypeInfoCache().Get("alt::Vector3")->GetTypeId())
                {
                    auto vector = *static_cast<Data::Vector3*>(value);
                    return core.CreateMValueVector3({ vector.x, vector.y, vector.z });
                }
                else if(type == runtime.GetTypeInfoCache().Get("alt::Vector2")->GetTypeId())
                {
                    auto vector = *static_cast<Data::Vector2*>(value);
                    return core.CreateMValueVector2({ vector.x, vector.y });
                }
                else if(type == runtime.GetTypeInfoCache().Get("alt::RGBA")->GetTypeId())
                {
                    auto rgba = *static_cast<alt::RGBA*>(value);
                    return core.CreateMValueRGBA(rgba);
                }
                else if(type == runtime.GetTypeInfoCache().Get("dictionary")->GetTypeId())
                {
                    auto dict = static_cast<CScriptDictionary*>(value);
                    auto list = core.CreateMValueDict();
                    for(auto it = dict->begin(); it != dict->end(); it++)
                    {
                        list->Set(it.GetKey(), ValueToMValue(it.GetTypeId(), const_cast<void*>(it.GetAddressOfValue())));
                    }
                    return list;
                }
                else if(type == runtime.GetTypeInfoCache().Get("array<uint8>")->GetTypeId())
                {
                    std::vector<uint8_t> arr;
                    auto                 byteArray = static_cast<CScriptArray*>(value);
                    for(asUINT i = 0; i < byteArray->GetSize(); i++)
                    {
                        arr.push_back(*(uint8_t*)byteArray->At(i));
                    }
                    return core.CreateMValueByteArray(arr.data(), byteArray->GetSize());
                }
                else if(GetTypeName(type) == "array")
                {
                    auto mvalue = core.CreateMValueList();
                    auto arr    = static_cast<CScriptArray*>(value);
                    for(asUINT i = 0; i < arr->GetSize(); i++)
                    {
                        mvalue->Push(ValueToMValue(arr->GetElementTypeId(), arr->At(i)));
                    }
                    return mvalue;
                }
                else if(type == runtime.GetTypeInfoCache().Get("alt::BaseObject@")->GetTypeId() ||
                        type == runtime.GetTypeInfoCache().Get("alt::WorldObject@")->GetTypeId() ||
                        type == runtime.GetTypeInfoCache().Get("alt::Entity@")->GetTypeId() ||
                        type == runtime.GetTypeInfoCache().Get("alt::Player@")->GetTypeId() ||
                        type == runtime.GetTypeInfoCache().Get("alt::Vehicle@")->GetTypeId())
                    return core.CreateMValueBaseObject(static_cast<alt::IBaseObject*>(value));

                break;
            }
        }

        // If the specified type is unknown, just return a nil value
        Log::Warning << "Invalid value type for MValue conversion: " << runtime.GetEngine()->GetTypeInfoById(type)->GetName() << " (" << type << ")"
                     << Log::Endl;
        return core.CreateMValueNone();
    }

    class MValueFunc : public alt::IMValueFunction::Impl
    {
        AngelScriptResource* resource;
        asIScriptFunction*   func;

    public:
        MValueFunc(AngelScriptResource* resource, asIScriptFunction* func) : resource(resource), func(func) {}

        alt::MValue Call(alt::MValueArgs args) const override
        {
            auto                               context = resource->GetContext();
            std::vector<std::pair<void*, int>> funcArgs;
            for(uint32_t i = 0; i < args.GetSize(); i++)
            {
                int  ret;
                auto arg         = args[i];
                auto [type, val] = MValueToValue(resource->GetRuntime(), arg);
                funcArgs.push_back({ val, type });
            }
            void* result = CallFunction(context, func, funcArgs);
            auto  value  = ValueToMValue(func->GetReturnTypeId(), result);
            for(auto [ptr, typeId] : funcArgs)
            {
                if(typeId != -1 && typeId != AngelScriptRuntime::Instance().GetTypeInfoCache().Get("alt::BaseObject@")->GetTypeId()) delete ptr;
            }
            return value;
        }
    };

    static std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter, size_t max = 0)
    {
        std::vector<std::string> parts;
        size_t                   last = 0, next = 0;
        size_t                   idx = 0;
        while((next = input.find(delimiter, last)) != std::string::npos)
        {
            parts.push_back(input.substr(last, next - last));
            last = next + 1;
            idx++;
            if(max != 0 && max - 1 == idx) break;
        }
        parts.push_back(input.substr(last));
        return parts;
    }

    static std::string GenerateRandomString(uint32_t size)
    {
        std::string availableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        std::random_device                    random_device;
        std::mt19937                          generator(random_device());
        std::uniform_int_distribution<size_t> distribution(0, availableChars.size() - 1);

        std::string random_string;
        for(std::size_t i = 0; i < size; ++i)
        {
            random_string += availableChars[distribution(generator)];
        }

        return random_string;
    }

    // Checks if all primitive type args of the function are marked as &in ref values,
    // otherwise it might cause a crash
    static inline bool CheckEventFunctionParams(asIScriptFunction* func)
    {
        asUINT paramCount = func->GetParamCount();
        for(asUINT i = 0; i < paramCount; i++)
        {
            int     typeId = 0;
            asDWORD flags  = 0;
            func->GetParam(i, &typeId, &flags);
            if(!IsTypePrimitive(typeId)) continue;
            if(flags == asETypeModifiers::asTM_INREF) continue;
            Log::Error << "Parameter " << i << " of event handler function '" << func->GetDeclaration()
                       << "' is a primitive and has to be a '&in' ref value" << Log::Endl;
            return false;
        }
        return true;
    }
}  // namespace Helpers
