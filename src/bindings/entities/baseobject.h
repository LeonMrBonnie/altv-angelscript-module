#pragma once
#include "Log.h"
#include "../../helpers/module.h"
#include "../../helpers/convert.h"
#include "../../helpers/angelscript.h"

using namespace Helpers;

template<class T>
static void AddRef(T* obj)
{
    obj->AddRef();
}

template<class T>
static void RemoveRef(T* obj)
{
    obj->RemoveRef();
}

template<class T>
static void GetMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();
    if(!obj->HasMetaData(key))
    {
        THROW_ERROR("The specified meta key does not exist on the object");
        return;
    }
    
    auto mvalue = obj->GetMetaData(key);
    auto value = Helpers::MValueToValue(resource->GetRuntime(), mvalue);

    auto engine = resource->GetRuntime()->GetEngine();
    Helpers::CopyAngelscriptValue(engine, value.second, value.first, ref, typeId);
}

template<class T>
static void SetMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    auto mvalue = Helpers::ValueToMValue(typeId, ref);
    if(mvalue->GetType() == alt::IMValue::Type::NIL)
    {
        THROW_ERROR("Invalid value passed to SetMeta");
        return;
    }
    obj->SetMetaData(key, mvalue);
}

template<class T>
static void Destroy(T* obj)
{
    alt::ICore::Instance().DestroyBaseObject(obj);
}

template<class T>
static bool IsValid(T* obj)
{
    GET_RESOURCE();
    return resource->DoesEntityExist(obj);
}

namespace Helpers
{
    template<class T>
    static void RegisterAsBaseObject(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        engine->RegisterObjectBehaviour(type, asBEHAVE_ADDREF, "void f()", asFUNCTION(AddRef<T>), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour(type, asBEHAVE_RELEASE, "void f()", asFUNCTION(RemoveRef<T>), asCALL_CDECL_OBJLAST);

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint8", "type", (GenericWrapper<T, alt::IBaseObject, &alt::IBaseObject::GetType, alt::IBaseObject::Type>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "valid", IsValid<T>);

        REGISTER_METHOD_WRAPPER(type, "bool HasMeta(const string&in key)", (GenericWrapper<T, alt::IBaseObject, &alt::IBaseObject::HasMetaData, bool, std::string&>));
        REGISTER_METHOD_WRAPPER(type, "void GetMeta(const string&in key, ?&out outValue)", GetMeta<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetMeta(const string&in key, ?&in value)", SetMeta<T>);
        REGISTER_METHOD_WRAPPER(type, "void DeleteMeta(const string&in key)", (GenericWrapper<T, alt::IBaseObject, &alt::IBaseObject::DeleteMetaData, void, std::string&>));

        REGISTER_METHOD_WRAPPER(type, "void Destroy()", Destroy<T>);
    }
}
