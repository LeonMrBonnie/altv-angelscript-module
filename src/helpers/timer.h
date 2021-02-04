#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include <chrono>

class AngelScriptResource;
namespace Helpers
{
    class Timer
    {
        AngelScriptResource* resource;
        asIScriptFunction* callback;
        uint32_t interval;
        int64_t lastRun;
        bool once;

    public:
        Timer(AngelScriptResource* resource, asIScriptFunction* callback, uint32_t interval, int64_t curTime, bool once);

        bool Update(int64_t time);
    };
}
