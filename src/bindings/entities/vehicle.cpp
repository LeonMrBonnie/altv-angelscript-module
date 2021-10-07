#include "Log.h"
#include "../../helpers/module.h"
#include "entity.h"
#include "../data/vehicleNeon.h"

using namespace Helpers;
using namespace Data;

static std::string ToString(alt::IVehicle* vehicle)
{
    std::stringstream str;
    str << "Vehicle{ id: " << std::to_string(vehicle->GetID()) << ", model: " << std::to_string(vehicle->GetModel()) << " }";
    return str.str();
}

#ifdef SERVER_MODULE
static alt::IVehicle* VehicleFactory(uint32_t model, Vector3& pos, Vector3& rot)
{
    GET_RESOURCE();
    auto vehicle = alt::ICore::Instance().CreateVehicle(model, { pos.x, pos.y, pos.z }, { rot.x, rot.y, rot.z });
    AS_ASSERT(!vehicle.IsEmpty(), "Invalid model hash", nullptr);

    vehicle->AddRef();
    return vehicle.Get();
}
#endif

static alt::IPlayer* GetDriver(alt::IVehicle* vehicle)
{
    auto driver = vehicle->GetDriver();
    if(driver.IsEmpty()) return nullptr;
    return driver.Get();
}

static std::string GetNumberplateText(alt::IVehicle* vehicle)
{
    return vehicle->GetNumberplateText().ToString();
}

#ifdef SERVER_MODULE
static VehicleNeon GetActiveNeon(alt::IVehicle* vehicle)
{
    bool front, back, left, right;
    vehicle->GetNeonActive(&left, &right, &front, &back);
    return VehicleNeon(front, back, left, right);
}

static void SetActiveNeon(alt::IVehicle* vehicle, VehicleNeon& neon)
{
    vehicle->SetNeonActive(neon.left, neon.right, neon.front, neon.back);
}

static alt::IVehicle* GetAttached(alt::IVehicle* vehicle)
{
    auto attached = vehicle->GetAttached();
    if(attached.IsEmpty()) return nullptr;
    return attached.Get();
}

static alt::IVehicle* GetAttachedTo(alt::IVehicle* vehicle)
{
    auto attached = vehicle->GetAttachedTo();
    if(attached.IsEmpty()) return nullptr;
    return attached.Get();
}
#endif

#ifdef CLIENT_MODULE
static Data::Vector3 GetSpeedVector(alt::IVehicle* vehicle)
{
    auto speedVector = vehicle->GetSpeedVector();
    return Data::Vector3(speedVector[0], speedVector[1], speedVector[2]);
}
#endif

static Data::Vector3 GetVelocity(alt::IVehicle* vehicle)
{
    auto velocity = vehicle->GetVelocity();
    return Data::Vector3(velocity[0], velocity[1], velocity[2]);
}

static ModuleExtension playerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsEntity<alt::IVehicle>(engine, docs, "Vehicle");

#ifdef SERVER_MODULE
    REGISTER_FACTORY("Vehicle", "Hash model, Vector3&in pos, Vector3&in rot", VehicleFactory);
#endif

    // Implicit conversion to string
    REGISTER_METHOD_WRAPPER("Vehicle", "string opImplConv() const", ToString);

    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "Player@+", "driver", GetDriver);

    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "bool", "destroyed", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsDestroyed, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "modKitsCount", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetModKitsCount, uint8_t>));
    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "uint8", "modKit", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetModKit, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "modKit", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetModKit, bool, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "hasCustomPrimaryColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsPrimaryColorRGB, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "primaryColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetPrimaryColor, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "primaryColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetPrimaryColor, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "RGBA", "primaryColorRGB", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetPrimaryColorRGB, alt::RGBA>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "RGBA", "primaryColorRGB", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetPrimaryColorRGB, void, alt::RGBA>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "hasCustomSecondaryColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsSecondaryColorRGB, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "secondaryColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetSecondaryColor, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "secondaryColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetSecondaryColor, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "RGBA", "secondaryColorRGB", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetSecondaryColorRGB, alt::RGBA>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "RGBA", "secondaryColorRGB", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetSecondaryColorRGB, void, alt::RGBA>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "pearlColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetPearlColor, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "pearlColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetPearlColor, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "wheelColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetWheelColor, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "wheelColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWheelColor, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "interiorColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetInteriorColor, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "interiorColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetInteriorColor, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "dashboardColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetDashboardColor, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "dashboardColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetDashboardColor, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "hasCustomTireSmokeColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsTireSmokeColorCustom, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "RGBA", "tireSmokeColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetTireSmokeColor, alt::RGBA>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "RGBA", "tireSmokeColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetTireSmokeColor, void, alt::RGBA>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "wheelType", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetWheelType, uint8_t>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "wheelVariation", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetWheelVariation, uint8_t>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "rearWheelVariation", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetRearWheelVariation, uint8_t>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "customTires", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetCustomTires, bool>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "bool", "customTires", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetCustomTires, void, bool>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "specialDarkness", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetSpecialDarkness, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "specialDarkness", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetSpecialDarkness, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint", "numberplateIndex", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetNumberplateIndex, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint", "numberplateIndex", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetNumberplateIndex, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "string", "numberplateText", GetNumberplateText);
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "string", "numberplateText", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetNumberplateText, void, std::string&>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "windowTint", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetWindowTint, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "windowTint", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWindowTint, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "dirtLevel", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetDirtLevel, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "dirtLevel", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetDirtLevel, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "neonActive", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsNeonActive, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "RGBA", "neonColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetNeonColor, alt::RGBA>));
    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "uint8", "livery", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetLivery, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "livery", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetLivery, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "roofLivery", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetRoofLivery, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "roofLivery", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetRoofLivery, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "bool", "engineOn", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsEngineOn, bool>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "bool", "engineOn", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetEngineOn, void, bool>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "handbrakeActive", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsHandbrakeActive, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "headlightColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetHeadlightColor, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "headlightColor", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetHeadlightColor, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint", "radioStation", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetRadioStationIndex, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint", "radioStation", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetRadioStationIndex, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "sirenActive", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsSirenActive, bool>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "bool", "sirenActive", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetSirenActive, void, bool>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "lockState", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetLockState, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "lockState", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetLockState, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "daylightOn", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsDaylightOn, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "nightlightOn", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsNightlightOn, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "roofState", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetRoofState, uint8_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "roofState", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetRoofState, void, uint8_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "flamethrowerActive", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsFlamethrowerActive, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "float", "lightsMultiplier", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetLightsMultiplier, float>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "float", "lightsMultiplier", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetLightsMultiplier, void, float>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint", "engineHealth", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetEngineHealth, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint", "engineHealth", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetEngineHealth, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint", "petrolTankHealth", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetPetrolTankHealth, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint", "petrolTankHealth", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetPetrolTankHealth, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "wheelsCount", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetWheelsCount, uint8_t>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "repairsCount", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetRepairsCount, uint8_t>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint", "bodyHealth", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetBodyHealth, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint", "bodyHealth", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetBodyHealth, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint", "additionalBodyHealth", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetBodyAdditionalHealth, uint32_t>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET("Vehicle",
                                  "uint",
                                  "additionalBodyHealth",
                                  (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetBodyAdditionalHealth, void, uint32_t>));
#endif
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "armoredWindows", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::HasArmoredWindows, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "manualEngineControl", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsManualEngineControl, bool>));
#ifdef SERVER_MODULE
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "bool", "manualEngineControl", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetManualEngineControl, void, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "Vehicle@+", "attached", GetAttached);
    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "Vehicle@+", "attachedTo", GetAttachedTo);
#endif

#ifdef SERVER_MODULE
    REGISTER_METHOD_WRAPPER(
      "Vehicle", "uint8 GetMod(uint8 category) const", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetMod, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool SetMod(uint8 category, uint8 mod)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetMod, bool, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "uint8 GetModsCount(uint8 category) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetModsCount, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER(
      "Vehicle", "bool IsExtraOn(uint8 extra) const", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsExtraOn, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetExtraOn(uint8 extra, bool toggle)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::ToggleExtra, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle", "VehicleNeon GetActiveNeon() const", GetActiveNeon);
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetActiveNeon(bool left, bool right, bool front, bool back)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetNeonActive, void, bool, bool, bool, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle", "void SetActiveNeon(VehicleNeon&in neon)", SetActiveNeon);
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "uint8 GetDoorState(uint8 door) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetDoorState, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetDoorState(uint8 door, uint8 state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetDoorState, void, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool IsWindowOpened(uint8 window) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsWindowOpened, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetWindowOpened(uint8 window, bool opened)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWindowOpened, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER(
      "Vehicle", "bool IsWheelBurst(uint8 wheel) const", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsWheelBurst, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetWheelBurst(uint8 wheel, bool state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWheelBurst, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool DoesWheelHaveTire(uint8 wheel) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::DoesWheelHasTire, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetWheelHasTire(uint8 wheel, bool state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWheelHasTire, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool IsWheelDetached(uint8 wheel) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsWheelDetached, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetWheelDetached(uint8 wheel, bool state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWheelDetached, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool IsWheelOnFire(uint8 wheel) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsWheelOnFire, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetWheelOnFire(uint8 wheel, bool state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWheelOnFire, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "float GetWheelHealth(uint8 wheel) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetWheelHealth, float, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetWheelHealth(uint8 wheel, float health)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWheelHealth, void, uint8_t, float>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "uint8 GetPartDamageLevel(uint8 part) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetPartDamageLevel, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetPartDamageLevel(uint8 part, uint8 level)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetPartDamageLevel, void, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "uint8 GetPartBulletholes(uint8 part) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetPartBulletHoles, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetPartBulletholes(uint8 part, uint8 amount)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetPartBulletHoles, void, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool IsLightDamaged(uint8 light) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsLightDamaged, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetLightDamaged(uint8 light, bool state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetLightDamaged, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool IsWindowDamaged(uint8 window) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsWindowDamaged, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetWindowDamaged(uint8 window, bool state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetWindowDamaged, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "bool IsSpecialLightDamaged(uint8 light) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsSpecialLightDamaged, bool, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetSpecialLightDamaged(uint8 light, bool state)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetSpecialLightDamaged, void, uint8_t, bool>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "float GetArmoredWindowHealth(uint8 window) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetArmoredWindowHealth, float, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "float SetArmoredWindowHealth(uint8 window, float health)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetArmoredWindowHealth, void, uint8_t, float>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "uint8 GetArmoredWindowBulletholes(uint8 window) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetArmoredWindowShootCount, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetArmoredWindowBulletholes(uint8 window, uint8 amount)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetArmoredWindowShootCount, void, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "uint8 GetBumperDamageLevel(uint8 bumper) const",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetBumperDamageLevel, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle",
                            "void SetBumperDamageLevel(uint8 bumper, uint8 level)",
                            (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetBumperDamageLevel, void, uint8_t, uint8_t>));
    REGISTER_METHOD_WRAPPER("Vehicle", "void Repair()", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetFixed, void>));
#endif

#ifdef CLIENT_MODULE
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "float", "wheelSpeed", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetWheelSpeed, float>));

    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint16", "gear", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetCurrentGear, uint16_t>));
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint16", "gear", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetCurrentGear, void, uint16_t>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint16", "maxGear", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetMaxGear, uint16_t>));
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint16", "maxGear", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetMaxGear, void, uint16_t>));

    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "float", "rpm", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetCurrentRPM, float>));
    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "Vector3", "speedVector", GetSpeedVector);

    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "uint8", "lightsIndicator", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::GetLightsIndicator, uint8_t>));
    REGISTER_PROPERTY_WRAPPER_SET(
      "Vehicle", "uint8", "lightsIndicator", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::SetLightsIndicator, void, uint8_t>));

    REGISTER_METHOD_WRAPPER("Vehicle", "void ResetHandling()", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::ResetHandling>));
    REGISTER_METHOD_WRAPPER("Vehicle", "void ReplaceHandling()", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::ReplaceHandling>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "Vehicle", "bool", "handlingModified", (GenericWrapper<alt::IVehicle, alt::IVehicle, &alt::IVehicle::IsHandlingModified, bool>));
#endif

    REGISTER_PROPERTY_WRAPPER_GET("Vehicle", "Vector3", "velocity", GetVelocity);
});
