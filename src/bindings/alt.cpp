#include "Log.h"
#include "../helpers/module.h"

using namespace Helpers;

static void Log(std::string& msg)
{
    Log::Colored << msg << Log::Endl;
}

static void LogWarning(std::string& msg)
{
    Log::Warning << msg << Log::Endl;
}

static void LogError(std::string& msg)
{
    Log::Error << msg << Log::Endl;
}

static uint32_t SetTimeout(asIScriptFunction* callback, uint32_t timeout)
{
    GET_RESOURCE();
    return resource->CreateTimer(timeout, callback, true);
}

static uint32_t SetInterval(asIScriptFunction* callback, uint32_t interval)
{
    GET_RESOURCE();
    return resource->CreateTimer(interval, callback, false);
}

static uint32_t NextTick(asIScriptFunction* callback)
{
    GET_RESOURCE();
    return resource->CreateTimer(0, callback, true);
}

static uint32_t EveryTick(asIScriptFunction* callback)
{
    GET_RESOURCE();
    return resource->CreateTimer(0, callback, false);
}

static void ClearTimer(uint32_t id)
{
    GET_RESOURCE();
    resource->RemoveTimer(id);
}

static void On(std::string& event, asIScriptFunction* callback)
{
    GET_RESOURCE();
    resource->RegisterEventHandler(event, callback);
}

static ModuleExtension altExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs)
{
    // Generic
    FUNCDEF("void GenericCallback()", "Generic callback without arguments or return value");

    // Logging
    GLOBAL_FUNC("void log(string msg)", Log, "Logs the specified message to the console");
    GLOBAL_FUNC("void logWarning(string msg)", LogWarning, "Logs the specified message as a warning to the console");
    GLOBAL_FUNC("void logError(string msg)", LogError, "Logs the specified message as an error to the console");

    // Timers
    GLOBAL_FUNC("uint setTimeout(GenericCallback@ callback, uint timeout)", SetTimeout, "Sets a timeout");
    GLOBAL_FUNC("uint setInterval(GenericCallback@ callback, uint interval)", SetInterval, "Sets a interval");
    GLOBAL_FUNC("uint nextTick(GenericCallback@ callback)", NextTick, "Sets a next tick handler");
    GLOBAL_FUNC("uint everyTick(GenericCallback@ callback)", EveryTick, "Sets a every tick handler");
    GLOBAL_FUNC("void clearTimer(uint timerId)", ClearTimer, "Clears specified timer");

    // Event handling
    GLOBAL_FUNC("void on(string event, GenericCallback@ callback)", On, "Registers a event handler");
});
