#include "helpers/events.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::RESOURCE_START, ResourceStart, "const string &in resource", 
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CResourceStartEvent*>(event);
    args.push_back(std::pair((void*)ev->GetResource()->GetName().CStr(), false));
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::RESOURCE_STOP, ResourceStop, "const string &in resource",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CResourceStopEvent*>(event);
    args.push_back(std::pair((void*)ev->GetResource()->GetName().CStr(), false));
});
