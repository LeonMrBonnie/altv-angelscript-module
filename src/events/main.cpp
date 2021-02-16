#include "helpers/events.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "../runtime.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::RESOURCE_START, ResourceStart, "void", "const string &in resource", 
[](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
    auto ev = static_cast<const alt::CResourceStartEvent*>(event);

    context->SetArgObject(0, (void*)ev->GetResource()->GetName().CStr());
    return context->Execute();
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::RESOURCE_STOP, ResourceStop, "void", "const string &in resource",
[](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
    auto ev = static_cast<const alt::CResourceStopEvent*>(event);

    context->SetArgObject(0, (void*)ev->GetResource()->GetName().CStr());
    return context->Execute();
});

REGISTER_EVENT_HANDLER(alt::CEvent::Type::CONSOLE_COMMAND_EVENT, ConsoleCommand, "void", "string command, array<string> args",
[](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
    auto ev = static_cast<const alt::CConsoleCommandEvent*>(event);

    auto evArgs = ev->GetArgs();
    auto arr = resource->GetRuntime()->CreateStringArray((uint32_t)evArgs.GetSize());
    for(int i = 0; i < evArgs.GetSize(); i++)
    {
        arr->SetValue(i, &evArgs[i].ToString());
    }
    context->SetArgObject(0, (void*)ev->GetName().CStr());
    context->SetArgObject(1, arr);
    return context->Execute();
});
