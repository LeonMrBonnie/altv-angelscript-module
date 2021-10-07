#pragma once
#include "Log.h"
#include "../../helpers/module.h"
#include "entity.h"
#include "../../helpers/angelscript.h"

using namespace Helpers;

static std::string GetName(alt::IPlayer* player)
{
    return player->GetName().ToString();
}

static CScriptArray* GetWeaponComponents(alt::IPlayer* player)
{
    GET_RESOURCE();
    auto components = player->GetCurrentWeaponComponents();
    auto arr        = resource->GetRuntime()->CreateUIntArray(components.GetSize());
    for(int i = 0; i < components.GetSize(); i++)
    {
        arr->SetValue(i, &components[i]);
    }
    return arr;
}

static Data::Vector3 GetAimPos(alt::IPlayer* player)
{
    alt::Vector3f vector = player->GetAimPos();
    return Data::Vector3(vector[0], vector[1], vector[2]);
}

static Data::Vector3 GetHeadRotation(alt::IPlayer* player)
{
    alt::Vector3f vector = player->GetHeadRotation();
    return Data::Vector3(vector[0], vector[1], vector[2]);
}

static alt::IVehicle* GetVehicle(alt::IPlayer* player)
{
    auto vehicle = player->GetVehicle();
    if(vehicle.IsEmpty()) return nullptr;
    return vehicle.Get();
}

static alt::IEntity* GetEntityAimingAt(alt::IPlayer* player)
{
    auto entity = player->GetEntityAimingAt();
    if(entity.IsEmpty()) return nullptr;
    return entity.Get();
}

static Data::Vector3 GetEntityAimOffset(alt::IPlayer* player)
{
    alt::Vector3f vector = player->GetEntityAimOffset();
    return Data::Vector3(vector[0], vector[1], vector[2]);
}

#pragma region Server
#ifdef SERVER_MODULE
static std::string GetIP(alt::IPlayer* player)
{
    return player->GetIP().ToString();
}

static std::string GetAuthToken(alt::IPlayer* player)
{
    return player->GetAuthToken().ToString();
}

static void Emit(asIScriptGeneric* gen)
{
    GET_RESOURCE();
    alt::IPlayer*   player = static_cast<alt::IPlayer*>(gen->GetObject());
    void*           ref    = gen->GetArgAddress(0);
    int             typeId = 0;
    std::string     event  = *static_cast<std::string*>(ref);
    alt::MValueArgs args;

    for(int i = 1; i < gen->GetArgCount(); i++)
    {
        ref    = gen->GetArgAddress(i);
        typeId = gen->GetArgTypeId(i);
        if(typeId & asTYPEID_OBJHANDLE)
        {
            // We're receiving a reference to the handle, so we need to dereference it
            ref = *(void**)ref;
            resource->GetRuntime()->GetEngine()->AddRefScriptObject(ref, resource->GetRuntime()->GetEngine()->GetTypeInfoById(typeId));
        }
        if(typeId == asTYPEID_VOID) continue;
        auto mvalue = Helpers::ValueToMValue(typeId, ref);
        args.Push(mvalue);
    }
    alt::ICore::Instance().TriggerClientEvent(player, event, args);
}

template<class T>
static void SpawnPlayer(alt::IPlayer* player, T x, T y, T z, uint32_t delay = 0)
{
    player->Spawn({ x, y, z }, delay);
}

static void SpawnPlayerVector(alt::IPlayer* player, Data::Vector3& pos, uint32_t delay = 0)
{
    player->Spawn({ pos.x, pos.y, pos.z }, delay);
}
#endif
#pragma endregion

namespace Helpers
{
    template<class T>
    static void RegisterAsPlayer(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsEntity<T>(engine, docs, type);

        REGISTER_PROPERTY_WRAPPER_GET(type, "string", "name", GetName);

#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_SET(type, "Hash", "model", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetModel, void, uint32_t>));
#endif

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "health", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetHealth, uint32_t>));
#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_SET(type, "uint", "health", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetHealth, void, uint32_t>));
#endif
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "maxHealth", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetMaxHealth, uint32_t>));
#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_SET(type, "uint", "maxHealth", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetMaxHealth, void, uint32_t>));
#endif

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "armour", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetArmour, uint32_t>));
#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_SET(type, "uint", "armour", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetArmour, void, uint32_t>));
#endif
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "maxArmour", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetMaxArmour, uint32_t>));
#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_SET(type, "uint", "maxArmour", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetMaxArmour, void, uint32_t>));
#endif

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "weapon", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetCurrentWeapon, uint32_t>));
#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_SET(type, "uint", "weapon", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetCurrentWeapon, void, uint32_t>));
#endif
        REGISTER_PROPERTY_WRAPPER_GET(type, "array<uint>@", "weaponComponents", GetWeaponComponents);
        REGISTER_PROPERTY_WRAPPER_GET(
          type, "uint8", "weaponTint", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetCurrentWeaponTintIndex, uint8_t>));

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "dead", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsDead, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "jumping", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsJumping, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "inRagdoll", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsInRagdoll, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "aiming", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsAiming, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "shooting", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsShooting, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "reloading", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsReloading, bool>));
#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "connected", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsConnected, bool>));
#endif

        REGISTER_PROPERTY_WRAPPER_GET(type, "float", "moveSpeed", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetMoveSpeed, float>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "Vector3", "aimPos", GetAimPos);
        REGISTER_PROPERTY_WRAPPER_GET(type, "Vector3", "headRot", GetHeadRotation);

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "inVehicle", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsInVehicle, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "Vehicle@+", "vehicle", GetVehicle);
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint8", "seat", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetSeat, uint8_t>));

        REGISTER_PROPERTY_WRAPPER_GET(type, "Entity@+", "entityAimingAt", GetEntityAimingAt);
        REGISTER_PROPERTY_WRAPPER_GET(type, "Vector3", "entityAimOffset", GetEntityAimOffset);

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "flashlightActive", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsFlashlightActive, bool>));

#ifdef SERVER_MODULE
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "ping", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetPing, uint32_t>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "string", "ip", GetIP);
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint64", "socialId", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetSocialID, uint64_t>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint64", "hwid", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetHwidHash, uint64_t>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint64", "hwidEx", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetHwidExHash, uint64_t>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "string", "authToken", GetAuthToken);

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "invincible", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetInvincible, bool>));
        REGISTER_PROPERTY_WRAPPER_SET(type, "bool", "invincible", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetInvincible, void, bool>));

        REGISTER_METHOD_WRAPPER(type, "void Spawn(float x, float y, float z, uint delay = 0)", SpawnPlayer<float>);
        REGISTER_METHOD_WRAPPER(type, "void Spawn(int x, int y, int z, uint delay = 0)", SpawnPlayer<int>);
        REGISTER_METHOD_WRAPPER(type, "void Spawn(Vector3&in pos, uint delay = 0)", SpawnPlayerVector);
        REGISTER_METHOD_WRAPPER(type, "void Despawn()", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::Despawn>));
#endif

        REGISTER_METHOD_WRAPPER(type,
                                "bool HasWeaponComponent(uint weapon, uint component) const",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::HasWeaponComponent, bool, uint32_t, uint32_t>));
        REGISTER_METHOD_WRAPPER(
          type, "uint GetWeaponTint(uint weapon) const", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetWeaponTintIndex, uint32_t, uint32_t>));
#ifdef SERVER_MODULE
        REGISTER_METHOD_WRAPPER(type,
                                "void SetWeaponTint(uint weapon, uint8 tint)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetWeaponTintIndex, void, uint32_t, uint8_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "void AddWeaponComponent(uint weapon, uint component)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::AddWeaponComponent, void, uint32_t, uint32_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "void RemoveWeaponComponent(uint weapon, uint component)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::RemoveWeaponComponent, void, uint32_t, uint32_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "void GiveWeapon(uint weapon, int ammo, bool equipNow = false)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GiveWeapon, void, uint32_t, int32_t, bool>));
        REGISTER_METHOD_WRAPPER(
          type, "bool RemoveWeapon(uint weapon)", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::RemoveWeapon, bool, uint32_t>));
        REGISTER_METHOD_WRAPPER(type, "void RemoveAllWeapons()", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::RemoveAllWeapons>));
#endif

#ifdef SERVER_MODULE
        REGISTER_METHOD_WRAPPER(type, "void ClearBloodDamage()", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::ClearBloodDamage>));
        REGISTER_METHOD_WRAPPER(
          type,
          "void SetDateTime(uint8 day, uint8 month, uint8 year, uint8 hour, uint8 minute, uint8 second)",
          (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetDateTime, void, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t>));
        REGISTER_METHOD_WRAPPER(type, "void SetWeather(uint8 weather)", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetWeather, void, uint8_t>));
        REGISTER_METHOD_WRAPPER(
          type, "void Kick(const string&in reason = \"Kicked\")", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::Kick, void, const std::string&>));
        REGISTER_METHOD_WRAPPER(type,
                                "bool IsEntityInStreamRange(Entity@ entity) const",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsEntityInStreamingRange, bool, alt::IEntity*>));
#endif

#ifdef SERVER_MODULE
        REGISTER_VARIADIC_METHOD(type, "void", "Emit", "const string&in event", 32, Emit);

        REGISTER_METHOD_WRAPPER(
          type, "Cloth GetClothes(uint8 component) const", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetClothes, alt::Cloth, uint8_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "DlcCloth GetDlcClothes(uint8 component) const",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetDlcClothes, alt::DlcCloth, uint8_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "void SetClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetClothes, void, uint8_t, uint16_t, uint8_t, uint8_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "void SetDlcClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetDlcClothes, void, uint8_t, uint16_t, uint8_t, uint8_t, uint32_t>));

        REGISTER_METHOD_WRAPPER(
          type, "Prop GetProps(uint8 component) const", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetProps, alt::Prop, uint8_t>));
        REGISTER_METHOD_WRAPPER(
          type, "DlcProp GetDlcProps(uint8 component) const", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetDlcProps, alt::DlcProp, uint8_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "void SetProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetProps, void, uint8_t, uint16_t, uint8_t>));
        REGISTER_METHOD_WRAPPER(type,
                                "void SetDlcProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetDlcProps, void, uint8_t, uint16_t, uint8_t, uint32_t>));

        REGISTER_METHOD_WRAPPER(type,
                                "void SetIntoVehicle(Vehicle@ vehicle, uint8 seat)",
                                (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetIntoVehicle, void, alt::IVehicle*, uint8_t>));
#endif

#ifdef CLIENT_MODULE
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "isTalking", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsTalking, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "float", "micLevel", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetMicLevel, float>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "float", "spatialVolume", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetSpatialVolume, float>));
        REGISTER_PROPERTY_WRAPPER_SET(
          type, "float", "spatialVolume", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetSpatialVolume, void, float>));
        REGISTER_PROPERTY_WRAPPER_GET(
          type, "float", "nonSpatialVolume", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::GetNonSpatialVolume, float>));
        REGISTER_PROPERTY_WRAPPER_SET(
          type, "float", "nonSpatialVolume", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::SetNonSpatialVolume, void, float>));
#endif

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "isSuperJumpEnabled", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsSuperJumpEnabled, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "isCrouching", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsCrouching, bool>));
        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "isStealthy", (GenericWrapper<T, alt::IPlayer, &alt::IPlayer::IsStealthy, bool>));
    }
}  // namespace Helpers
