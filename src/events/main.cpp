#include "helpers/events.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "../runtime.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::RESOURCE_START,
  ResourceStart,
  "void",
  "const string &in resource",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CResourceStartEvent*>(event);

      context->SetArgObject(0, (void*)ev->GetResource()->GetName().CStr());
      return context->Execute();
  },
  EventFlag::SHARED);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::RESOURCE_STOP,
  ResourceStop,
  "void",
  "const string &in resource",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CResourceStopEvent*>(event);

      context->SetArgObject(0, (void*)ev->GetResource()->GetName().CStr());
      return context->Execute();
  },
  EventFlag::SHARED);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::CONSOLE_COMMAND_EVENT,
  ConsoleCommand,
  "void",
  "const string&in command, array<string> args",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CConsoleCommandEvent*>(event);

      auto evArgs = ev->GetArgs();
      auto arr    = resource->GetRuntime()->CreateStringArray((uint32_t)evArgs.GetSize());
      for(int i = 0; i < evArgs.GetSize(); i++)
      {
          auto val = evArgs[i].ToString();
          arr->SetValue(i, &val);
      }
      context->SetArgObject(0, (void*)ev->GetName().CStr());
      context->SetArgObject(1, arr);
      return context->Execute();
  },
  EventFlag::SHARED);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::COLSHAPE_EVENT,
  ColshapeStateChange,
  "void",
  "ColShape@ colshape, Entity@ entity, bool&in state",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CColShapeEvent*>(event);

      context->SetArgObject(0, (void*)ev->GetTarget().Get());
      context->SetArgObject(1, (void*)ev->GetEntity().Get());
      auto state = ev->GetState();
      context->SetArgAddress(2, &state);
      return context->Execute();
  },
  EventFlag::SERVER_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::CONNECTION_COMPLETE,
  ConnectionComplete,
  "void",
  "",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) { return context->Execute(); },
  EventFlag::CLIENT_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::DISCONNECT_EVENT,
  Disconnect,
  "void",
  "",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) { return context->Execute(); },
  EventFlag::CLIENT_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::KEYBOARD_EVENT,
  KeyPress,
  "void",
  "uint&in key, KeyState&in state",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev  = static_cast<const alt::CKeyboardEvent*>(event);
      auto key = ev->GetKeyCode();
      context->SetArgAddress(0, &key);
      auto state = ev->GetKeyState();
      context->SetArgAddress(1, &state);
      return context->Execute();
  },
  EventFlag::CLIENT_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::GAME_ENTITY_CREATE,
  GameEntityCreate,
  "void",
  "Entity@ entity",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CGameEntityCreateEvent*>(event);
      context->SetArgObject(0, ev->GetTarget().Get());
      return context->Execute();
  },
  EventFlag::CLIENT_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::GAME_ENTITY_DESTROY,
  GameEntityDestroy,
  "void",
  "Entity@ entity",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
      auto ev = static_cast<const alt::CGameEntityDestroyEvent*>(event);
      context->SetArgObject(0, ev->GetTarget().Get());
      return context->Execute();
  },
  EventFlag::CLIENT_ONLY);

REGISTER_EVENT_HANDLER(
  alt::CEvent::Type::RENDER,
  Render,
  "void",
  "",
  [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) { return context->Execute(); },
  EventFlag::CLIENT_ONLY);
