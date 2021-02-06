#include "helpers/events.h"

using namespace Helpers;

static Event resourceStartEvent(alt::CEvent::Type::RESOURCE_START, "resourceStart", "const string &in resource", 
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CResourceStartEvent*>(event);
    args.push_back(std::pair((void*)ev->GetResource()->GetName().CStr(), false));
},
[](AngelScriptResource* resource, const alt::CEvent* event, std::string name) {
    return resource->GetEventHandlers(name);
});

static Event resourceStopEvent(alt::CEvent::Type::RESOURCE_STOP, "resourceStop", "const string &in resource", 
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CResourceStopEvent*>(event);
    args.push_back(std::pair((void*)ev->GetResource()->GetName().CStr(), false));
},
[](AngelScriptResource* resource, const alt::CEvent* event, std::string name) {
    return resource->GetEventHandlers(name);
}, true);
