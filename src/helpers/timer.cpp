#include "timer.h"
#include "../resource.h"
#include "angelscript.h"

using namespace Helpers;

Timer::Timer(AngelScriptResource* resource, asIScriptFunction* callback, uint32_t interval, int64_t curTime, bool once)
    : resource(resource), callback(callback), lastRun(curTime), interval(interval), once(once)
{
}

bool Timer::Update(int64_t time)
{
    auto elapsed = time - lastRun;
    // If the interval has been reached, run the timer callback
    if(elapsed >= interval)
    {
        auto context = resource->GetContext();
        context->Prepare(callback);
        int r = context->Execute();
        CHECK_FUNCTION_RETURN(r, false);

        lastRun = time;

        return !once;
    }

    return true;
}
