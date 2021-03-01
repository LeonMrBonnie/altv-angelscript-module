#pragma once
#include "Log.h"
#include "../../helpers/module.h"
#include "worldObject.h"
#include "../../helpers/angelscript.h"

using namespace Helpers;

template<class T>
static Vector3 GetRotation(T* obj)
{
    alt::Vector3f rot = obj->GetRotation();
    return Vector3(rot[0], rot[1], rot[2]);
}

template<class T>
static void SetRotation(T* obj, Vector3& pos)
{
    obj->SetRotation(alt::Rotation{pos.x, pos.y, pos.z});
}

template<class T>
static alt::IPlayer* GetNetOwner(T* obj)
{
    alt::Ref<alt::IPlayer> player = obj->GetNetworkOwner();
    if(player.IsEmpty()) return nullptr;
    return player.Get();
}

template<class T>
static void SetNetOwner(T* obj, alt::IPlayer* player, bool disableMigration)
{
    obj->SetNetworkOwner(alt::Ref<alt::IPlayer>(player), disableMigration);
}

template<class T>
static bool GetSyncedMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();
    if(!obj->HasSyncedMetaData(key)) return false;
    
    auto mvalue = obj->GetSyncedMetaData(key);
    auto [type, ptr] = Helpers::MValueToValue(resource->GetRuntime(), mvalue);

    auto engine = resource->GetRuntime()->GetEngine();
    Helpers::CopyAngelscriptValue(engine, ptr, type, ref, typeId);
    return true;
}

template<class T>
static void SetSyncedMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    auto mvalue = Helpers::ValueToMValue(typeId, ref);
    if(mvalue->GetType() == alt::IMValue::Type::NIL)
    {
        THROW_ERROR("Invalid value passed to SetSyncedMeta");
        return;
    }
    obj->SetSyncedMetaData(key, mvalue);
}

template<class T>
static bool GetStreamSyncedMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();
    if(!obj->HasStreamSyncedMetaData(key)) return false;
    
    auto mvalue = obj->GetStreamSyncedMetaData(key);
    auto [type, ptr] = Helpers::MValueToValue(resource->GetRuntime(), mvalue);

    auto engine = resource->GetRuntime()->GetEngine();
    Helpers::CopyAngelscriptValue(engine, ptr, type, ref, typeId);
    return true;
}

template<class T>
static void SetStreamSyncedMeta(T* obj, const std::string& key, void* ref, int typeId)
{
    auto mvalue = Helpers::ValueToMValue(typeId, ref);
    if(mvalue->GetType() == alt::IMValue::Type::NIL)
    {
        THROW_ERROR("Invalid value passed to SetStreamSyncedMeta");
        return;
    }
    obj->SetStreamSyncedMetaData(key, mvalue);
}

namespace Helpers
{
    template<class T>
    static void RegisterAsEntity(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsWorldObject<T>(engine, docs, type);

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint16", "id", (GenericWrapper<T, alt::IEntity, &alt::IEntity::GetID, uint16_t>));

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "model", (GenericWrapper<T, alt::IEntity, &alt::IEntity::GetModel, uint32_t>));

        REGISTER_PROPERTY_WRAPPER_GET(type, "Vector3", "rot", GetRotation<T>);
        REGISTER_PROPERTY_WRAPPER_SET(type, "Vector3", "rot", SetRotation<T>);

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "visible", (GenericWrapper<T, alt::IEntity, &alt::IEntity::GetVisible, bool>));
        REGISTER_PROPERTY_WRAPPER_SET(type, "bool", "visible", (GenericWrapper<T, alt::IEntity, &alt::IEntity::SetVisible, void, bool>));

        REGISTER_METHOD_WRAPPER(type, "Player@+ GetNetOwner() const", GetNetOwner<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetNetOwner(Player@ player, bool disableMigration = false)", SetNetOwner<T>);

        REGISTER_METHOD_WRAPPER(type, "bool HasSyncedMeta(const string&in key)", (GenericWrapper<T, alt::IEntity, &alt::IEntity::HasSyncedMetaData, bool, std::string&>));
        REGISTER_METHOD_WRAPPER(type, "bool GetSyncedMeta(const string&in key, ?&out outValue)", GetSyncedMeta<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetSyncedMeta(const string&in key, ?&in value)", SetSyncedMeta<T>);
        REGISTER_METHOD_WRAPPER(type, "void DeleteSyncedMeta(const string&in key)", (GenericWrapper<T, alt::IEntity, &alt::IEntity::DeleteSyncedMetaData, void, std::string&>));

        REGISTER_METHOD_WRAPPER(type, "bool HasStreamSyncedMeta(const string&in key)", (GenericWrapper<T, alt::IEntity, &alt::IEntity::HasStreamSyncedMetaData, bool, std::string&>));
        REGISTER_METHOD_WRAPPER(type, "bool GetStreamSyncedMeta(const string&in key, ?&out outValue)", GetStreamSyncedMeta<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetStreamSyncedMeta(const string&in key, ?&in value)", SetStreamSyncedMeta<T>);
        REGISTER_METHOD_WRAPPER(type, "void DeleteStreamSyncedMeta(const string&in key)", (GenericWrapper<T, alt::IEntity, &alt::IEntity::DeleteStreamSyncedMetaData, void, std::string&>));
    }
}
