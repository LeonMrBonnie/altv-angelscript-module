#include "helpers/events.h"
#include "cpp-sdk/events/CPlayerWeaponChangeEvent.h"
#include "../bindings/vector3.h"
#include "cpp-sdk/events/CPlayerEnteringVehicleEvent.h"
#include "cpp-sdk/events/CPlayerLeaveVehicleEvent.h"
#include "cpp-sdk/events/CPlayerChangeVehicleSeatEvent.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_CONNECT, PlayerConnect, "void", "Player@ player",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerConnectEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_DISCONNECT, PlayerDisconnect, "void", "Player@ player, string reason",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerDisconnectEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({(void*)ev->GetReason().CStr(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_DAMAGE, PlayerDamage, "void", "Player@ player, Entity@ attacker, uint damage, uint weapon",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerDamageEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({ev->GetAttacker().Get(), false});
    args.push_back({(void*)ev->GetDamage(), true});
    args.push_back({(void*)ev->GetWeapon(), true});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_DEATH, PlayerDeath, "void", "Player@ player, Entity@ killer, uint weapon",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerDeathEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({ev->GetKiller().Get(), false});
    args.push_back({(void*)ev->GetWeapon(), true});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_WEAPON_CHANGE, PlayerWeaponChange, "void", "Player@ player, uint oldWeapon, uint newWeapon",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerWeaponChangeEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({(void*)ev->GetOldWeapon(), true});
    args.push_back({(void*)ev->GetNewWeapon(), true});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::WEAPON_DAMAGE_EVENT, WeaponDamage, "void", "Player@ source, Entity@ target, uint weapon, uint damage, Vector3f offset, uint bodyPart",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CWeaponDamageEvent*>(event);
    args.push_back({ev->GetSource().Get(), false});
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({(void*)ev->GetWeaponHash(), true});
    args.push_back({(void*)ev->GetDamageValue(), true});
    auto offset = ev->GetShotOffset();
    args.push_back({&Vector3<float>(offset[0], offset[1], offset[2]), false});
    args.push_back({(void*)ev->GetBodyPart(), true});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_ENTER_VEHICLE, PlayerEnteredVehicle, "void", "Player@ player, Vehicle@ vehicle, uint seat",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(event);
    args.push_back({ev->GetPlayer().Get(), false});
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({(void*)ev->GetSeat(), true});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_ENTERING_VEHICLE, PlayerEnteringVehicle, "void", "Player@ player, Vehicle@ vehicle, uint seat",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerEnteringVehicleEvent*>(event);
    args.push_back({ev->GetPlayer().Get(), false});
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({(void*)ev->GetSeat(), true});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_LEAVE_VEHICLE, PlayerLeaveVehicle, "void", "Player@ player, Vehicle@ vehicle, uint seat",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(event);
    args.push_back({ev->GetPlayer().Get(), false});
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({(void*)ev->GetSeat(), true});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_CHANGE_VEHICLE_SEAT, PlayerChangedVehicleSeat, "void", "Player@ player, Vehicle@ vehicle, uint oldSeat, uint newSeat",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(event);
    args.push_back({ev->GetPlayer().Get(), false});
    args.push_back({ev->GetTarget().Get(), false});
    args.push_back({(void*)ev->GetOldSeat(), true});
    args.push_back({(void*)ev->GetNewSeat(), true});
});
