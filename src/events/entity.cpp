#include "helpers/events.h"
#include "angelscript/addon/scriptarray/scriptarray.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::REMOVE_ENTITY_EVENT, RemoveEntity, "void", "Entity@ entity",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CRemoveEntityEvent*>(event);
    args.push_back({ev->GetEntity().Get(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::REMOVE_BASE_OBJECT_EVENT, RemoveBaseObject, "void", "BaseObject@ object",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CRemoveBaseObjectEvent*>(event);
    args.push_back({ev->GetObject().Get(), false});
});
