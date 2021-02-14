#pragma once
#include "Log.h"
#include "../helpers/module.h"
#include "entity.h"
#include "../runtime.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "../helpers/convert.h"

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

template<class T>
static void SpawnPlayer(T x, T y, T z, uint32_t delay, alt::IPlayer* player)
{
    player->Spawn({x, y, z}, delay);
}

static void SetModel(uint32_t model, alt::IPlayer* player)
{
    player->SetModel(model);
}

static uint32_t GetHealth(alt::IPlayer* player)
{
    return player->GetHealth();
}

static void SetHealth(uint32_t value, alt::IPlayer* player)
{
    player->SetHealth(value);
}

static uint32_t GetMaxHealth(alt::IPlayer* player)
{
    return player->GetMaxHealth();
}

static void SetMaxHealth(uint32_t value, alt::IPlayer* player)
{
    player->SetMaxHealth(value);
}

static uint32_t GetArmour(alt::IPlayer* player)
{
    return player->GetArmour();
}

static void SetArmour(uint32_t value, alt::IPlayer* player)
{
    player->SetArmour(value);
}

static uint32_t GetMaxArmour(alt::IPlayer* player)
{
    return player->GetMaxArmour();
}

static void SetMaxArmour(uint32_t value, alt::IPlayer* player)
{
    player->SetMaxArmour(value);
}

static bool HasWeaponComponent(uint32_t weapon, uint32_t comp, alt::IPlayer* player)
{
    return player->HasWeaponComponent(weapon, comp);
}

static CScriptArray* GetWeaponComponents(alt::IPlayer* player)
{
    GET_RESOURCE();
    auto components = player->GetCurrentWeaponComponents();
    auto arr = resource->GetRuntime()->CreateUIntArray(components.GetSize());
    for(int i = 0; i < components.GetSize(); i++)
    {
        arr->SetValue(i, &components[i]);
    }
    return arr;
}

static uint32_t GetWeaponTint(uint32_t weapon, alt::IPlayer* player)
{
    return player->GetWeaponTintIndex(weapon);
}

static uint32_t GetCurrentWeaponTint(alt::IPlayer* player)
{
    return player->GetCurrentWeaponTintIndex();
}

static uint32_t GetCurrentWeapon(alt::IPlayer* player)
{
    return player->GetCurrentWeapon();
}

static void SetCurrentWeapon(uint32_t weapon, alt::IPlayer* player)
{
    player->SetCurrentWeapon(weapon);
}

static bool IsDead(alt::IPlayer* player)
{
    return player->IsDead();
}

static bool IsJumping(alt::IPlayer* player)
{
    return player->IsJumping();
}

static bool IsInRagdoll(alt::IPlayer* player)
{
    return player->IsInRagdoll();
}

static bool IsAiming(alt::IPlayer* player)
{
    return player->IsAiming();
}

static bool IsShooting(alt::IPlayer* player)
{
    return player->IsShooting();
}

static bool IsReloading(alt::IPlayer* player)
{
    return player->IsReloading();
}

static float GetMoveSpeed(alt::IPlayer* player)
{
    return player->GetMoveSpeed();
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

static bool IsInVehicle(alt::IPlayer* player)
{
    return player->IsInVehicle();
}

static alt::IVehicle* GetVehicle(alt::IPlayer* player)
{
    auto vehicle = player->GetVehicle();
    if(vehicle.IsEmpty()) return nullptr;
    return vehicle.Get();
}

static uint32_t GetSeat(alt::IPlayer* player)
{
    return player->GetSeat();
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

static bool IsFlashlightActive(alt::IPlayer* player)
{
    return player->IsFlashlightActive();
}

static void Despawn(alt::IPlayer* player)
{
    player->Despawn();
}

static bool IsConnected(alt::IPlayer* player)
{
    return player->IsConnected();
}

static uint32_t GetPing(alt::IPlayer* player)
{
    return player->GetPing();
}

static std::string GetIP(alt::IPlayer* player)
{
    return player->GetIP().ToString();
}

static uint64_t GetSocialID(alt::IPlayer* player)
{
    return player->GetSocialID();
}

static uint64_t GetHwidHash(alt::IPlayer* player)
{
    return player->GetHwidHash();
}

static uint64_t GetHwidExHash(alt::IPlayer* player)
{
    return player->GetHwidExHash();
}

static std::string GetAuthToken(alt::IPlayer* player)
{
    return player->GetAuthToken().ToString();
}

static void Emit(const std::string& event, CScriptArray* args, alt::IPlayer* player)
{
    GET_RESOURCE();
    alt::MValueArgs mvalueArgs;
    for(int i = 0; i < args->GetSize(); i++)
    {
        CScriptAny* arg = (CScriptAny*)args->At(i);
        void* value = nullptr;
        arg->Retrieve(&value, arg->GetTypeId());
        if(value == nullptr)
        {
            THROW_ERROR("Invalid args passed");
            return;
        }
        mvalueArgs.Push(Helpers::ValueToMValue(arg->GetTypeId(), value));
    }
    alt::ICore::Instance().TriggerClientEvent(alt::Ref<alt::IPlayer>(player), event, mvalueArgs);
}

static ModuleExtension playerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsEntity<alt::IPlayer>(engine, docs, "Player");

    // Implicit conversion to string
    REGISTER_METHOD_WRAPPER("Player", "string opImplConv() const", ToString);

    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "name", GetName);

    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "model", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetModel, void, uint32_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "health", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetHealth, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "health", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetHealth, void, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "maxHealth", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetMaxHealth, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "maxHealth", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetMaxHealth, void, uint32_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "armour", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetArmour, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "armour", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetArmour, void, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "maxArmour", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetMaxArmour, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "maxArmour", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetMaxArmour, void, uint32_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "weapon", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetCurrentWeapon, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "weapon", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetCurrentWeapon, void, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "array<uint>@", "weaponComponents", GetWeaponComponents);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint8", "weaponTint", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetCurrentWeaponTintIndex, uint8_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "dead", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsDead, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "jumping", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsJumping, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "inRagdoll", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsInRagdoll, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "aiming", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsAiming, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "shooting", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsShooting, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "reloading", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsReloading, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "connected", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsConnected, bool>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "float", "moveSpeed", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetMoveSpeed, float>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vector3", "aimPos", GetAimPos);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vector3", "headRot", GetHeadRotation);

    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "inVehicle", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsInVehicle, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vehicle@", "vehicle", GetVehicle);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint8", "seat", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetSeat, uint8_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "Entity@", "entityAimingAt", GetEntityAimingAt);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "Vector3", "entityAimOffset", GetEntityAimOffset);

    REGISTER_PROPERTY_WRAPPER_GET("Player", "bool", "flashlightActive", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsFlashlightActive, bool>));

    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint", "ping", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetPing, uint32_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "ip", GetIP);
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint64", "socialId", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetSocialID, uint64_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint64", "hwid", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetHwidHash, uint64_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "uint64", "hwidEx", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetHwidExHash, uint64_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "authToken", GetAuthToken);
    
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(float x, float y, float z, uint delay)", SpawnPlayer<float>);
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(int x, int y, int z, uint delay)", SpawnPlayer<int>);
    REGISTER_METHOD_WRAPPER("Player", "void Despawn()", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::Despawn>));

    REGISTER_METHOD_WRAPPER("Player", "bool HasWeaponComponent(uint weapon, uint component)", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::HasWeaponComponent, bool, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player", "uint GetWeaponTint(uint weapon)", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetWeaponTintIndex, uint32_t, uint32_t>));

    REGISTER_METHOD_WRAPPER("Player", "void Emit(const string&in event, array<any>@ args)", Emit);

    // todo: add missing methods
});
