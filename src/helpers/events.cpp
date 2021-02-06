#include "events.h"

using namespace Helpers;

std::unordered_map<alt::CEvent::Type, Event*> Event::all = std::unordered_map<alt::CEvent::Type, Event*>();

void Helpers::OnEvent(std::string& event, asIScriptFunction* callback)
{
    GET_RESOURCE();
    resource->RegisterEventHandler(event, callback);
}
