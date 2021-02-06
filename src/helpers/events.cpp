#include "events.h"

using namespace Helpers;

std::unordered_map<alt::CEvent::Type, Event*> Event::all = std::unordered_map<alt::CEvent::Type, Event*>();
