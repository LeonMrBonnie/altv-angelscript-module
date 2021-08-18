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
static bool GetMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();
    if(!obj->HasMetaData(key)) return false;

    auto mvalue      = obj->GetMetaData(key);
    auto [type, ptr] = Helpers::MValueToValue(resource->GetRuntime(), mvalue);

    auto engine = resource->GetRuntime()->GetEngine();
    Helpers::CopyAngelscriptValue(engine, ptr, type, ref, typeId);
    return true;
}

template<class T>
static void SetMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    auto mvalue = Helpers::ValueToMValue(typeId, ref);
    AS_ASSERT(mvalue->GetType() != alt::IMValue::Type::NIL, "Invalid value passed", );

    obj->SetMetaData(key, mvalue);
}

template<class T>
static bool HasData(T* obj, const std::string& key)
{
    GET_RESOURCE();
    return resource->HasObjectData(obj, key);
}

template<class T>
static bool GetData(T* obj, const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();
    if(!resource->HasObjectData(obj, key)) return false;

    auto [type, value] = resource->GetObjectData(obj, key);

    Helpers::CopyAngelscriptValue(resource->GetRuntime()->GetEngine(), value, type, ref, typeId);
    return true;
}

template<class T>
static void SetData(T* obj, const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();

    resource->SetObjectData(obj, key, typeId, obj);
}

template<class T>
static void DeleteData(T* obj, const std::string& key)
{
    GET_RESOURCE();
    if(!resource->HasObjectData(obj, key)) return;

    resource->DeleteObjectData(obj, key);
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
    return resource->DoesObjectExist(obj);
}

namespace Helpers
{
    template<class T>
    static void RegisterAsBaseObject(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        engine->RegisterObjectBehaviour(type, asBEHAVE_ADDREF, "void f()", asFUNCTION(AddRef<T>), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour(type, asBEHAVE_RELEASE, "void f()", asFUNCTION(RemoveRef<T>), asCALL_CDECL_OBJLAST);

        REGISTER_PROPERTY_WRAPPER_GET(
          type, "uint8", "type", (GenericWrapper<T, alt::IBaseObject, &alt::IBaseObject::GetType, alt::IBaseObject::Type>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "valid", IsValid<T>);

        REGISTER_METHOD_WRAPPER(
          type, "bool HasMeta(const string&in key) const", (GenericWrapper<T, alt::IBaseObject, &alt::IBaseObject::HasMetaData, bool, std::string&>));
        REGISTER_METHOD_WRAPPER(type, "bool GetMeta(const string&in key, ?&out outValue) const", GetMeta<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetMeta(const string&in key, ?&in value)", SetMeta<T>);
        REGISTER_METHOD_WRAPPER(
          type, "void DeleteMeta(const string&in key)", (GenericWrapper<T, alt::IBaseObject, &alt::IBaseObject::DeleteMetaData, void, std::string&>));

        REGISTER_METHOD_WRAPPER(type, "bool HasData(const string&in key) const", HasData<T>);
        REGISTER_METHOD_WRAPPER(type, "bool GetData(const string&in key, ?&out outValue) const", GetData<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetData(const string&in key, ?&in value)", SetData<T>);
        REGISTER_METHOD_WRAPPER(type, "void DeleteData(const string&in key)", DeleteData<T>);

        REGISTER_METHOD_WRAPPER(type, "void Destroy()", Destroy<T>);
    }
}  // namespace Helpers
