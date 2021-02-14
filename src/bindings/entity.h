#pragma once
#include "Log.h"
#include "../helpers/module.h"
#include "worldobject.h"

using namespace Helpers;

template<class T>
static uint32_t GetID(T* obj)
{
    return obj->GetID();
}

template<class T>
static uint32_t GetModel(T* obj)
{
    return obj->GetModel();
}

template<class T>
static Vector3 GetRotation(T* obj)
{
    alt::Vector3f rot = obj->GetRotation();
    return Vector3(rot[0], rot[1], rot[2]);
}

template<class T>
static void SetRotation(Vector3 pos, T* obj)
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
static void SetNetOwner(alt::IPlayer* player, bool disableMigration, T* obj)
{
    obj->SetNetworkOwner(alt::Ref<alt::IPlayer>(player), disableMigration);
}

template<class T>
static bool GetVisible(T* obj)
{
    return obj->GetVisible();
}

template<class T>
static void SetVisible(bool toggle, T* obj)
{
    obj->SetVisible(toggle);
}

namespace Helpers
{
    template<class T>
    static void RegisterAsEntity(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsWorldObject<T>(engine, docs, type);

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "id", GetID<T>);

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "model", GetModel<T>);

        REGISTER_PROPERTY_WRAPPER_GET(type, "Vector3", "rot", GetRotation<T>);
        REGISTER_PROPERTY_WRAPPER_SET(type, "Vector3", "rot", SetRotation<T>);

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "visible", GetVisible<T>);
        REGISTER_PROPERTY_WRAPPER_SET(type, "bool", "visible", SetVisible<T>);

        REGISTER_METHOD_WRAPPER(type, "Player@+ GetNetOwner() const", GetNetOwner<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetNetOwner(Player@ player, bool disableMigration = false)", SetNetOwner<T>);

        // todo: add missing methods
    }
}
