#include "helpers/events.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "cpp-sdk/events/CVehicleAttachEvent.h"
#include "cpp-sdk/events/CVehicleDetachEvent.h"
#include "cpp-sdk/events/CNetOwnerChangeEvent.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::VEHICLE_DESTROY,
  VehicleDestroy,
  "void",
  "Vehicle@ vehicle",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CVehicleDestroyEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::VEHICLE_ATTACH,
  VehicleAttach,
  "void",
  "Vehicle@ vehicle, Vehicle@ attachedVehicle",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CVehicleAttachEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      context->SetArgObject(1, ev->GetAttached().Get());
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::VEHICLE_DETACH,
  VehicleDetach,
  "void",
  "Vehicle@ vehicle, Vehicle@ detachedVehicle",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CVehicleDetachEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      context->SetArgObject(1, ev->GetDetached().Get());
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::NETOWNER_CHANGE,
  NetOwnerChange,
  "void",
  "Vehicle@ vehicle, Player@ oldOwner, Player@ newOwner",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CNetOwnerChangeEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      context->SetArgObject(1, ev->GetOldOwner().Get());
      context->SetArgObject(2, ev->GetNewOwner().Get());
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);
