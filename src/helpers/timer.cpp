#include "timer.h"
#include "../resource.h"
#include "angelscript.h"

using namespace Helpers;

Timer::Timer(AngelScriptResource* resource, asIScriptFunction* callback, asIScriptObject* object, uint32_t interval, int64_t curTime, bool once)
    : resource(resource), callback(callback), object(object), lastRun(curTime), interval(interval), once(once)
{
}

bool Timer::Update(int64_t time)
{
    auto elapsed = time - lastRun;
    // If the interval has been reached, run the timer callback
    if(elapsed >= interval)
    {
        auto context = resource->GetContext();
        int  r       = context->Prepare(callback);
        CHECK_AS_RETURN("Timer prepare callback", r, false);
        if(object)
        {
            r = context->SetObject(object);
            CHECK_AS_RETURN("Timer set object", r, false);
        }
        r = context->Execute();
        CHECK_AS_RETURN("Timer execute callback", r, false);

        lastRun = time;

        return !once;
    }

    return true;
}
