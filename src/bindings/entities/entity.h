#pragma once
#include "Log.h"
#include "../../helpers/module.h"
#include "worldObject.h"

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

        // todo: add missing methods
    }
}
