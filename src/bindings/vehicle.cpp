#pragma once
#include "Log.h"
#include "../helpers/module.h"
#include "entity.h"

using namespace Helpers;

static alt::IVehicle* VehicleFactory(uint32_t model, Vector3<float> pos, Vector3<float> rot)
{
    GET_RESOURCE();
    auto vehicle = alt::ICore::Instance().CreateVehicle(model, {pos.x, pos.y, pos.z}, {rot.x, rot.y, rot.z});
    if(vehicle.IsEmpty())
    {
        THROW_ERROR("Invalid model hash");
        return nullptr;
    }
    vehicle->AddRef();
    return vehicle.Get();
}

static ModuleExtension playerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsEntity<alt::IVehicle>(engine, docs, "Vehicle");
    REGISTER_FACTORY("Vehicle", "uint model, Vector3f pos, Vector3f rot", VehicleFactory);

    // todo: add missing methods
});
