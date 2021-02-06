#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "helpers/timer.h"

class AngelScriptRuntime;
class AngelScriptResource : public alt::IResource::Impl
{  
    AngelScriptRuntime* runtime;
    alt::IResource* resource;
    asIScriptModule* module;
    asIScriptContext* context;

    std::unordered_map<uint32_t, Helpers::Timer*> timers;
    std::vector<uint32_t> invalidTimers;
    uint32_t nextTimerId = 1;

    std::vector<std::pair<alt::CEvent::Type, asIScriptFunction*>> eventHandlers;

public:
    AngelScriptResource(AngelScriptRuntime* runtime, alt::IResource* resource) : runtime(runtime), resource(resource) {};
    ~AngelScriptResource() = default;

    alt::IResource* GetResource()
    {
        return resource;
    }
    asIScriptContext* GetContext()
    {
        return context;
    }
    asIScriptModule* GetModule()
    {
        return module;
    }

    alt::String ReadFile(alt::String path);
    void RegisterEventHandler(alt::CEvent::Type event, asIScriptFunction* handler)
    {
        eventHandlers.emplace_back(std::pair(event, handler));
    }
    std::vector<asIScriptFunction*> GetEventHandlers(alt::CEvent::Type event)
    {
        std::vector<asIScriptFunction*> events;
        for(auto handler : eventHandlers)
        {
            if(handler.first == event) events.push_back(handler.second);
        }
        return events;
    }

    uint32_t CreateTimer(uint32_t timeout, asIScriptFunction* callback, bool once)
    {

        uint32_t id = nextTimerId++;
        timers[id] = new Helpers::Timer{this, callback, timeout, GetTime(), once};

        return id;
    }
    void RemoveTimer(uint32_t id)
    {
        invalidTimers.emplace_back(id);
    }

    // Yoinked from v8 helpers
    int64_t GetTime()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

    bool Start();
    bool Stop();

    bool OnEvent(const alt::CEvent* event);
    void OnTick();

    void OnCreateBaseObject(alt::IBaseObject* object) {}
    void OnRemoveBaseObject(alt::IBaseObject* object) {}
};

