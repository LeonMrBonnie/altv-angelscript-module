#include "events.h"
#include "./angelscript.h"

using namespace Helpers;

std::unordered_map<alt::CEvent::Type, Event*> Event::all = std::unordered_map<alt::CEvent::Type, Event*>();

bool Helpers::Event::InvokeEventHandlers(AngelScriptResource*            resource,
                                         const alt::CEvent*              ev,
                                         std::vector<asIScriptFunction*> handlers,
                                         asIScriptObject*                object)
{
    // If the return type of the event is bool, it should return a value
    bool              shouldReturn = strcmp(GetReturnType(), "bool") == 0;
    asIScriptContext* context      = resource->GetContext();
    bool              cancel       = false;

    for(auto callback : handlers)
    {
        auto r = context->Prepare(callback);
        CHECK_AS_RETURN("Prepare event handler", r, true);
        if(object)
        {
            r = context->SetObject(object);
            CHECK_AS_RETURN("Set script class event method object", r, true);
        }
        // Set the event args and execute callback
        r = Execute(resource, ev);
        CHECK_FUNCTION_RETURN(r, true);
        if(r == asEXECUTION_FINISHED && shouldReturn)
        {
            if(callback->GetReturnTypeId() != asTYPEID_BOOL)
            {
                Log::Error << "Event handler '" << callback->GetDeclaration() << "' should return a bool" << Log::Endl;
                continue;
            }
            auto result = context->GetReturnByte();
            if(result == 1) cancel = true;
        }
    }
    context->Unprepare();

    return cancel;
}
