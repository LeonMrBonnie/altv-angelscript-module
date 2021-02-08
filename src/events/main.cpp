#include "helpers/events.h"
#include "angelscript/addon/scriptarray/scriptarray.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::RESOURCE_START, ResourceStart, "const string &in resource", 
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CResourceStartEvent*>(event);
    args.push_back({(void*)ev->GetResource()->GetName().CStr(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::RESOURCE_STOP, ResourceStop, "const string &in resource",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CResourceStopEvent*>(event);
    args.push_back({(void*)ev->GetResource()->GetName().CStr(), false});
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::CONSOLE_COMMAND_EVENT, ConsoleCommand, "string command, array<string> args",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CConsoleCommandEvent*>(event);

    auto evArgs = ev->GetArgs();
    auto arr = resource->CreateStringArray(evArgs.GetSize());
    for(int i = 0; i < evArgs.GetSize(); i++)
    {
        arr->SetValue(i, &evArgs[i].ToString());
    }

    args.push_back({(void*)ev->GetName().CStr(), false});
    args.push_back({arr, false});
});
