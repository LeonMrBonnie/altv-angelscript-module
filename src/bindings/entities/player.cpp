#include "Log.h"
#include "../../helpers/module.h"
#include "entity.h"
#include "../../runtime.h"
#include "../../helpers/convert.h"

using namespace Helpers;

static std::string ToString(alt::IPlayer* player)
{
    std::stringstream str;
    str << "Player{ id: " << std::to_string(player->GetID()) << ", name: " << player->GetName().ToString() << " }";
    return str.str();
}

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

static Vector3 GetAimPos(alt::IPlayer* player)
{
    alt::Vector3f vector = player->GetAimPos();
    return Vector3(vector[0], vector[1], vector[2]);
}

static Vector3 GetHeadRotation(alt::IPlayer* player)
{
    alt::Vector3f vector = player->GetHeadRotation();
    return Vector3(vector[0], vector[1], vector[2]);
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

static Vector3 GetEntityAimOffset(alt::IPlayer* player)
{
    alt::Vector3f vector = player->GetEntityAimOffset();
    return Vector3(vector[0], vector[1], vector[2]);
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
    alt::IPlayer*   player = static_cast<alt::IPlayer*>(asGetActiveContext()->GetThisPointer());
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

static void SpawnPlayerVector(alt::IPlayer* player, Vector3& pos, uint32_t delay = 0)
{
    player->Spawn({ pos.x, pos.y, pos.z }, delay);
}
#endif
#pragma endregion
#pragma region Client
#ifdef CLIENT_MODULE
static alt::IPlayer* GetLocalPlayer()
{
    return alt::ICore::Instance().GetLocalPlayer().Get();
}
#endif
#pragma endregion

static ModuleExtension playerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsEntity<alt::IPlayer>(engine, docs, "Player");

    // Implicit conversion to string
    REGISTER_METHOD_WRAPPER("Player", "string opImplConv() const", ToString);

    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "name", GetName);

#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "model", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetModel, void, uint32_t>));
#endif

    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "health", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetHealth, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "health", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetHealth, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "maxHealth", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetMaxHealth, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Player", "uint", "maxHealth", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetMaxHealth, void, uint32_t>));
#endif

    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "armour", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetArmour, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "armour", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetArmour, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "maxArmour", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetMaxArmour, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Player", "uint", "maxArmour", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetMaxArmour, void, uint32_t>));
#endif

    REGISTER_PROPERTY_WRAPPER_GET(
      "Player", "uint", "weapon", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetCurrentWeapon, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Player", "uint", "weapon", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetCurrentWeapon, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET("Player", "array<uint>@", "weaponComponents", GetWeaponComponents);
    REGISTER_PROPERTY_WRAPPER_GET(
      "Player", "uint8", "weaponTint", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetCurrentWeaponTintIndex, uint8_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "dead", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsDead, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "jumping", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsJumping, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "inRagdoll", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsInRagdoll, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "aiming", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsAiming, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "shooting", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsShooting, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "reloading", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsReloading, bool>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "connected", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsConnected, bool>));
#endif

    REGISTER_PROPERTY_WRAPPER_GET("Player", "float", "moveSpeed", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetMoveSpeed, float>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vector3", "aimPos", GetAimPos);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vector3", "headRot", GetHeadRotation);

    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "inVehicle", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsInVehicle, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vehicle@", "vehicle", GetVehicle);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint8", "seat", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetSeat, uint8_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "Entity@", "entityAimingAt", GetEntityAimingAt);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vector3", "entityAimOffset", GetEntityAimOffset);

    REGISTER_PROPERTY_WRAPPER_GET(
      "Player", "bool", "flashlightActive", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsFlashlightActive, bool>));

#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "ping", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetPing, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "ip", GetIP);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint64", "socialId", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetSocialID, uint64_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint64", "hwid", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetHwidHash, uint64_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint64", "hwidEx", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetHwidExHash, uint64_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "authToken", GetAuthToken);

    REGISTER_METHOD_WRAPPER("Player", "void Spawn(float x, float y, float z, uint delay = 0)", SpawnPlayer<float>);
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(int x, int y, int z, uint delay = 0)", SpawnPlayer<int>);
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(Vector3 pos, uint delay = 0)", SpawnPlayerVector);
    REGISTER_METHOD_WRAPPER("Player", "void Despawn()", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::Despawn>));
#endif

    REGISTER_METHOD_WRAPPER("Player",
                            "bool HasWeaponComponent(uint weapon, uint component)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::HasWeaponComponent, bool, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "uint GetWeaponTint(uint weapon)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetWeaponTintIndex, uint32_t, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_METHOD_WRAPPER("Player",
                            "void SetWeaponTint(uint weapon, uint8 tint)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetWeaponTintIndex, void, uint32_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "void AddWeaponComponent(uint weapon, uint component)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::AddWeaponComponent, void, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "void RemoveWeaponComponent(uint weapon, uint component)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::RemoveWeaponComponent, void, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "void GiveWeapon(uint weapon, int ammo, bool equipNow = false)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GiveWeapon, void, uint32_t, int32_t, bool>));
    REGISTER_METHOD_WRAPPER(
      "Player", "bool RemoveWeapon(uint weapon)", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::RemoveWeapon, bool, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player", "void RemoveAllWeapons()", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::RemoveAllWeapons>));
#endif

#ifdef SERVER_MODULE
    REGISTER_METHOD_WRAPPER("Player", "void ClearBloodDamage()", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::ClearBloodDamage>));
    REGISTER_METHOD_WRAPPER(
      "Player",
      "void SetDateTime(uint8 day, uint8 month, uint8 year, uint8 hour, uint8 minute, uint8 second)",
      (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetDateTime, void, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER(
      "Player", "void SetWeather(uint8 weather)", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetWeather, void, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "void Kick(const string&in reason = \"Kicked\")",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::Kick, void, const std::string&>));
    REGISTER_METHOD_WRAPPER("Player",
                            "bool IsEntityInStreamRange(Entity@ entity)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsEntityInStreamingRange, bool, alt::IEntity*>));
#endif

#ifdef SERVER_MODULE
    REGISTER_VARIADIC_METHOD("Player", "void", "Emit", "const string&in event", 32, Emit);

    REGISTER_METHOD_WRAPPER(
      "Player", "Cloth GetClothes(uint8 component)", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetClothes, alt::Cloth, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "DlcCloth GetDlcClothes(uint8 component)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetDlcClothes, alt::DlcCloth, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "void SetClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetClothes, void, uint8_t, uint16_t, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER(
      "Player",
      "void SetDlcClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc)",
      (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetDlcClothes, void, uint8_t, uint16_t, uint8_t, uint8_t, uint32_t>));

    REGISTER_METHOD_WRAPPER(
      "Player", "Prop GetProps(uint8 component)", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetProps, alt::Prop, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "DlcProp GetDlcProps(uint8 component)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetDlcProps, alt::DlcProp, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "void SetProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetProps, void, uint8_t, uint16_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player",
                            "void SetDlcProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc)",
                            (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetDlcProps, void, uint8_t, uint16_t, uint8_t, uint32_t>));
#endif

#ifdef CLIENT_MODULE
    REGISTER_GLOBAL_FUNC("Player@+ GetLocalPlayer()", GetLocalPlayer, "Gets the local player instance");

    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "isTalking", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsTalking, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "float", "micLevel", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetMicLevel, float>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Player", "float", "spatialVolume", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetSpatialVolume, float>));
    REGISTER_PROPERTY_WRAPPER_SET(
      "Player", "float", "spatialVolume", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetSpatialVolume, void, float>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Player", "float", "nonSpatialVolume", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetNonSpatialVolume, float>));
    REGISTER_PROPERTY_WRAPPER_SET(
      "Player", "float", "nonSpatialVolume", (GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetNonSpatialVolume, void, float>));
#endif
});
