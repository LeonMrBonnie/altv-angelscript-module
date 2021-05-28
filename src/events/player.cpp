#include "helpers/events.h"
#include "cpp-sdk/events/CPlayerWeaponChangeEvent.h"
#include "../bindings/data/vector3.h"
#include "cpp-sdk/events/CPlayerEnteringVehicleEvent.h"
#include "cpp-sdk/events/CPlayerLeaveVehicleEvent.h"
#include "cpp-sdk/events/CPlayerChangeVehicleSeatEvent.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_CONNECT,
  PlayerConnect,
  "void",
  "Player@ player",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerConnectEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_DISCONNECT,
  PlayerDisconnect,
  "void",
  "Player@ player, string reason",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerDisconnectEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      context->SetArgObject(1, (void*)ev->GetReason().CStr());
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_DAMAGE,
  PlayerDamage,
  "void",
  "Player@ player, Entity@ attacker, uint&in damage, uint&in weapon",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerDamageEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      context->SetArgObject(1, ev->GetAttacker().Get());
      auto dmg = ev->GetDamage();
      context->SetArgAddress(2, &dmg);
      auto weapon = ev->GetWeapon();
      context->SetArgAddress(3, &weapon);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_DEATH,
  PlayerDeath,
  "void",
  "Player@ player, Entity@ killer, uint&in weapon",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerDeathEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      context->SetArgObject(1, ev->GetKiller().Get());
      auto weapon = ev->GetWeapon();
      context->SetArgAddress(2, &weapon);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_WEAPON_CHANGE,
  PlayerWeaponChange,
  "void",
  "Player@ player, uint&in oldWeapon, uint&in newWeapon",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerWeaponChangeEvent*>(event);

      context->SetArgObject(0, ev->GetTarget().Get());
      auto old = ev->GetOldWeapon();
      context->SetArgAddress(1, &old);
      auto newWep = ev->GetNewWeapon();
      context->SetArgAddress(2, &newWep);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::WEAPON_DAMAGE_EVENT,
  WeaponDamage,
  "void",
  "Player@ source, Entity@ target, uint&in weapon, uint&in damage, Vector3 offset, uint&in bodyPart",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CWeaponDamageEvent*>(event);

      context->SetArgObject(0, ev->GetSource().Get());
      context->SetArgObject(1, ev->GetTarget().Get());
      auto hash = ev->GetWeaponHash();
      context->SetArgAddress(2, &hash);
      auto value = ev->GetDamageValue();
      context->SetArgAddress(3, &value);
      auto    offset = ev->GetShotOffset();
      Vector3 offsetVector(offset[0], offset[1], offset[2]);
      context->SetArgObject(4, &offsetVector);
      auto bodypart = ev->GetBodyPart();
      context->SetArgAddress(5, &bodypart);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_ENTER_VEHICLE,
  PlayerEnteredVehicle,
  "void",
  "Player@ player, Vehicle@ vehicle, uint8&in seat",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(event);

      context->SetArgObject(0, ev->GetPlayer().Get());
      context->SetArgObject(1, ev->GetTarget().Get());
      auto seat = ev->GetSeat();
      context->SetArgAddress(2, &seat);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_ENTERING_VEHICLE,
  PlayerEnteringVehicle,
  "void",
  "Player@ player, Vehicle@ vehicle, uint8&in seat",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerEnteringVehicleEvent*>(event);

      context->SetArgObject(0, ev->GetPlayer().Get());
      context->SetArgObject(1, ev->GetTarget().Get());
      auto seat = ev->GetSeat();
      context->SetArgAddress(2, &seat);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_LEAVE_VEHICLE,
  PlayerLeaveVehicle,
  "void",
  "Player@ player, Vehicle@ vehicle, uint8&in seat",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(event);

      context->SetArgObject(0, ev->GetPlayer().Get());
      context->SetArgObject(1, ev->GetTarget().Get());
      auto seat = ev->GetSeat();
      context->SetArgAddress(2, &seat);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::PLAYER_CHANGE_VEHICLE_SEAT,
  PlayerChangedVehicleSeat,
  "void",
  "Player@ player, Vehicle@ vehicle, uint&in oldSeat, uint&in newSeat",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(event);

      context->SetArgObject(0, ev->GetPlayer().Get());
      context->SetArgObject(1, ev->GetTarget().Get());
      auto oldSeat = ev->GetOldSeat();
      context->SetArgAddress(2, &oldSeat);
      auto newSeat = ev->GetNewSeat();
      context->SetArgAddress(3, &newSeat);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);
