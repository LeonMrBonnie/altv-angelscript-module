#include "helpers/events.h"
#include "angelscript/add_on/scriptarray/scriptarray.h"
#include "cpp-sdk/events/CVehicleAttachEvent.h"
#include "cpp-sdk/events/CVehicleDetachEvent.h"
#include "cpp-sdk/events/CNetOwnerChangeEvent.h"

using namespace Helpers;

#ifdef SERVER_MODULE
REGISTER_EVENT_HANDLER(alt::CEvent::Type::VEHICLE_DESTROY,
                       VehicleDestroy,
                       "void",
                       "Vehicle@ vehicle",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CVehicleDestroyEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::VEHICLE_ATTACH,
                       VehicleAttach,
                       "void",
                       "Vehicle@ vehicle, Vehicle@ attachedVehicle",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CVehicleAttachEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           context->SetArgObject(1, ev->GetAttached().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::VEHICLE_DETACH,
                       VehicleDetach,
                       "void",
                       "Vehicle@ vehicle, Vehicle@ detachedVehicle",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CVehicleDetachEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           context->SetArgObject(1, ev->GetDetached().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::NETOWNER_CHANGE,
                       NetOwnerChange,
                       "void",
                       "Vehicle@ vehicle, Player@ oldOwner, Player@ newOwner",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CNetOwnerChangeEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           context->SetArgObject(1, ev->GetOldOwner().Get());
                           context->SetArgObject(2, ev->GetNewOwner().Get());
                           return context->Execute();
                       });
#endif

#ifdef CLIENT_MODULE
REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_ENTER_VEHICLE,
                       VehicleEnter,
                       "void",
                       "Vehicle@ vehicle",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_LEAVE_VEHICLE,
                       VehicleLeave,
                       "void",
                       "Vehicle@ vehicle",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_CHANGE_VEHICLE_SEAT,
                       VehicleSeatChange,
                       "void",
                       "Vehicle@ vehicle, uint8&in newSeat, uint8&in oldSeat",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           auto seat = ev->GetNewSeat();
                           context->SetArgAddress(1, &seat);
                           auto oldSeat = ev->GetOldSeat();
                           context->SetArgAddress(2, &oldSeat);
                           return context->Execute();
                       });
#endif
