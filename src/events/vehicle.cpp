#include "helpers/events.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "cpp-sdk/events/CVehicleAttachEvent.h"
#include "cpp-sdk/events/CVehicleDetachEvent.h"
#include "cpp-sdk/events/CNetOwnerChangeEvent.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::VEHICLE_DESTROY, VehicleDestroy, "void", "Vehicle@ vehicle",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CVehicleDestroyEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::VEHICLE_ATTACH, VehicleAttach, "void", "Vehicle@ vehicle, Vehicle@ attachedVehicle",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CVehicleAttachEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({ev->GetAttached().Get(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::VEHICLE_DETACH, VehicleDetach, "void", "Vehicle@ vehicle, Vehicle@ detachedVehicle",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CVehicleDetachEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({ev->GetDetached().Get(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::NETOWNER_CHANGE, NetOwnerChange, "void", "Vehicle@ vehicle, Player@ oldOwner, Player@ newOwner",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CNetOwnerChangeEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({ev->GetOldOwner().Get(), false});
    args.push_back({ev->GetNewOwner().Get(), false});
});
