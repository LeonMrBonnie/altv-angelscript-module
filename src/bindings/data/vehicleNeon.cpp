#include "vehicleNeon.h"

using namespace Helpers;
using namespace Data;

#ifdef SERVER_MODULE
static void ConstructVehicleNeon(void* memory, bool front, bool back, bool left, bool right)
{
    new(memory) VehicleNeon(front, back, left, right);
}

static void DestructVehicleNeon(void* memory)
{
    ((VehicleNeon*)memory)->~VehicleNeon();
}

static DataExtension vehicleNeonExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_VALUE_CLASS("VehicleNeon", VehicleNeon, asOBJ_VALUE | asOBJ_POD, "The neon state of a vehicle");
    REGISTER_CONSTRUCTOR("VehicleNeon", "bool front, bool back, bool left, bool right", ConstructVehicleNeon);
    REGISTER_DESTRUCTOR("VehicleNeon", DestructVehicleNeon);
    REGISTER_PROPERTY("VehicleNeon", "const bool front", VehicleNeon, front);
    REGISTER_PROPERTY("VehicleNeon", "const bool back", VehicleNeon, back);
    REGISTER_PROPERTY("VehicleNeon", "const bool left", VehicleNeon, left);
    REGISTER_PROPERTY("VehicleNeon", "const bool right", VehicleNeon, right);
});
#endif
