#include "helpers/events.h"
#include "angelscript/addon/scriptarray/scriptarray.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::REMOVE_ENTITY_EVENT,
                       RemoveEntity,
                       "void",
                       "Entity@ entity",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CRemoveEntityEvent*>(event);

                           context->SetArgObject(0, ev->GetEntity().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::REMOVE_BASE_OBJECT_EVENT,
                       RemoveBaseObject,
                       "void",
                       "BaseObject@ object",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CRemoveBaseObjectEvent*>(event);

                           context->SetArgObject(0, ev->GetObject().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::CREATE_BASE_OBJECT_EVENT,
                       CreateBaseObject,
                       "void",
                       "BaseObject@ object",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CCreateBaseObjectEvent*>(event);

                           context->SetArgObject(0, ev->GetObject().Get());
                           return context->Execute();
                       });

#ifdef CLIENT_MODULE
REGISTER_EVENT_HANDLER(alt::CEvent::Type::GAME_ENTITY_CREATE,
                       GameEntityCreate,
                       "void",
                       "Entity@ entity",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CGameEntityCreateEvent*>(event);
                           context->SetArgObject(0, ev->GetTarget().Get());
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::GAME_ENTITY_DESTROY,
                       GameEntityDestroy,
                       "void",
                       "Entity@ entity",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CGameEntityDestroyEvent*>(event);
                           context->SetArgObject(0, ev->GetTarget().Get());
                           return context->Execute();
                       });
#endif
