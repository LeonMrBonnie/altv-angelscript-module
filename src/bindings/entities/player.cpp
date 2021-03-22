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

template<class T>
static void SpawnPlayer(alt::IPlayer* player, T x, T y, T z, uint32_t delay = 0)
{
    player->Spawn({x, y, z}, delay);
}

static void SpawnPlayerVector(alt::IPlayer* player, Vector3& pos, uint32_t delay = 0)
{
    player->Spawn({pos.x, pos.y, pos.z}, delay);
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
    alt::IPlayer* player = static_cast<alt::IPlayer*>(asGetActiveContext()->GetThisPointer());
    void* ref = gen->GetArgAddress(0);
	int typeId = 0;
	std::string event = *static_cast<std::string*>(ref);
    alt::MValueArgs args;
    
    for(int i = 1; i < gen->GetArgCount(); i++)
    {
        ref = gen->GetArgAddress(i);
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
    
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(float x, float y, float z, uint delay = 0)", SpawnPlayer<float>);
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(int x, int y, int z, uint delay = 0)", SpawnPlayer<int>);
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(Vector3 pos, uint delay = 0)", SpawnPlayerVector);
    REGISTER_METHOD_WRAPPER("Player", "void Despawn()", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::Despawn>));

    REGISTER_METHOD_WRAPPER("Player", "bool HasWeaponComponent(uint weapon, uint component)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::HasWeaponComponent, bool, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player", "uint GetWeaponTint(uint weapon)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetWeaponTintIndex, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player", "void SetWeaponTint(uint weapon, uint8 tint)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetWeaponTintIndex, void, uint32_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "void AddWeaponComponent(uint weapon, uint component)",
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::AddWeaponComponent, void, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player", "void RemoveWeaponComponent(uint weapon, uint component)",
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::RemoveWeaponComponent, void, uint32_t, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player", "void GiveWeapon(uint weapon, int ammo, bool equipNow = false)",
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GiveWeapon, void, uint32_t, int32_t, bool>));
    REGISTER_METHOD_WRAPPER("Player", "bool RemoveWeapon(uint weapon)",
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::RemoveWeapon, bool, uint32_t>));
    REGISTER_METHOD_WRAPPER("Player", "void RemoveAllWeapons()", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::RemoveAllWeapons>));

    REGISTER_METHOD_WRAPPER("Player", "void ClearBloodDamage()", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::ClearBloodDamage>));
    REGISTER_METHOD_WRAPPER("Player", "void SetDateTime(uint8 day, uint8 month, uint8 year, uint8 hour, uint8 minute, uint8 second)",
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetDateTime, void, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "void SetWeather(uint8 weather)", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetWeather, void, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "void Kick(const string&in reason = \"Kicked\")", (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::Kick, void, const std::string&>));
    REGISTER_METHOD_WRAPPER("Player", "bool IsEntityInStreamRange(Entity@ entity)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::IsEntityInStreamingRange, bool, alt::IEntity*>));

    REGISTER_VARIADIC_METHOD("Player", "void", "Emit", "const string&in event", 32, Emit);

    REGISTER_METHOD_WRAPPER("Player", "Cloth GetClothes(uint8 component)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetClothes, alt::Cloth, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "DlcCloth GetDlcClothes(uint8 component)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetDlcClothes, alt::DlcCloth, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "void SetClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetClothes, void, uint8_t, uint16_t, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "void SetDlcClothes(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetDlcClothes, void, uint8_t, uint16_t, uint8_t, uint8_t, uint32_t>));

    REGISTER_METHOD_WRAPPER("Player", "Prop GetProps(uint8 component)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetProps, alt::Prop, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "DlcProp GetDlcProps(uint8 component)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::GetDlcProps, alt::DlcProp, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "void SetProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette = 2)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetProps, void, uint8_t, uint16_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Player", "void SetDlcProps(uint8 component, uint16 drawable, uint8 texture, uint8 palette, uint dlc)", 
        (Helpers::GenericWrapper<alt::IPlayer, alt::IPlayer, &alt::IPlayer::SetDlcProps, void, uint8_t, uint16_t, uint8_t, uint32_t>));
});
