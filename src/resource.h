#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "helpers/timer.h"
#include "angelscript/include/angelscript.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/addon/scriptbuilder/scriptbuilder.h"
#include "angelscript/addon/scripthelper/scripthelper.h"
#include "cpp-sdk/events/CWebSocketClientEvent.h"

class AngelScriptRuntime;
class AngelScriptResource : public alt::IResource::Impl
{
    using ObjectsMap    = std::unordered_multimap<alt::IBaseObject::Type, alt::Ref<alt::IBaseObject>>;
    using ObjectDataMap = std::unordered_map<alt::Ref<alt::IBaseObject>, std::map<std::string, std::pair<int, void*>>>;

    AngelScriptRuntime* runtime;
    alt::IResource*     resource;
    asIScriptModule*    module  = nullptr;
    asIScriptContext*   context = nullptr;

    ObjectsMap    objects;
    ObjectDataMap objectData;

    std::vector<asIScriptObject*> scriptClasses = {};

    // Timers
    std::unordered_map<uint32_t, Helpers::Timer*> timers;
    std::vector<uint32_t>                         invalidTimers;
    uint32_t                                      nextTimerId = 1;

    // first = event type, second = script callback
    std::unordered_multimap<alt::CEvent::Type, asIScriptFunction*> eventHandlers;
    // first = custom event name, second = script callback
    std::unordered_multimap<std::string, asIScriptFunction*> customLocalEventHandlers;
    std::unordered_multimap<std::string, asIScriptFunction*> customRemoteEventHandlers;

    // Functions that should be evaled in next tick
    std::vector<asIScriptFunction*> evalFunctions;

    // Keep track of all imported functions, to release them on resource stop
    std::vector<asIScriptFunction*> importedFunctions;

    // Keep track of all library import functions, we need to remove them on resource stop
    std::vector<asIScriptFunction*> libraryImportFunctions;
    // Keep track of the imported libraries, to free them on resource stop
    std::vector<void*> importedLibraries;

public:
    AngelScriptResource(AngelScriptRuntime* runtime, alt::IResource* resource) : runtime(runtime), resource(resource){};
    ~AngelScriptResource() = default;

    bool MakeClient(alt::IResource::CreationInfo* info, alt::Array<alt::String> files)
    {
        // todo: change this to 'as' when client module is ready
        // info->type = "as";
        return true;
    }

    // Gets the alt:V IResource instance
    alt::IResource* GetIResource() const
    {
        return resource;
    }
    // Gets the module runtime that instantiated this resource
    AngelScriptRuntime* GetRuntime() const
    {
        return runtime;
    }
    // Gets the AngelScript context of this resource
    asIScriptContext* GetContext() const
    {
        return context;
    }
    // Gets the AngelScript module of this resource
    asIScriptModule* GetModule() const
    {
        return module;
    }

    void AddLibraryImportFunction(asIScriptFunction* func, void* lib)
    {
        libraryImportFunctions.push_back(func);
        importedLibraries.push_back(lib);
    }

    bool DoesObjectExist(alt::IBaseObject* obj) const
    {
        auto range = objects.equal_range(obj->GetType());
        for(auto it = range.first; it != range.second; it++)
        {
            if(it->second == obj) return true;
        }
        return false;
    }

    // Returns whether registering the metadata failed or not
    bool RegisterMetadata(CScriptBuilder& builder, asIScriptContext* context);
    void RegisterDefines(CScriptBuilder& builder);
    // Registers all the imported functions
    void RegisterImports();
    // Registers the resource exports
    void RegisterExports(CScriptBuilder& builder);

    alt::String ReadFile(alt::String path) const;

    // Registers a new script callback for the specified event
    void RegisterEventHandler(alt::CEvent::Type event, asIScriptFunction* handler);
    // Gets all script event handlers of the specified type
    std::vector<asIScriptFunction*> GetEventHandlers(const alt::CEvent::Type event) const
    {
        std::vector<asIScriptFunction*> events;
        auto                            range = eventHandlers.equal_range(event);
        for(auto it = range.first; it != range.second; it++) events.push_back(it->second);
        return events;
    }

    void RegisterCustomEventHandler(const std::string& name, asIScriptFunction* handler, bool local = true)
    {
        if(local) customLocalEventHandlers.insert({ name, handler });
        else
            customRemoteEventHandlers.insert({ name, handler });
    }
    std::vector<asIScriptFunction*> GetCustomEventHandlers(const std::string& name, bool local = true)
    {
        std::vector<asIScriptFunction*> arr;
        if(local)
        {
            auto range = customLocalEventHandlers.equal_range(name);
            for(auto it = range.first; it != range.second; it++) arr.push_back(it->second);
        }
        else
        {
            auto range = customRemoteEventHandlers.equal_range(name);
            for(auto it = range.first; it != range.second; it++) arr.push_back(it->second);
        }
        return arr;
    }
    void HandleCustomEvent(const alt::CEvent* event, bool local = true);
#ifdef CLIENT_MODULE
    void HandleWebviewEvent(const alt::CWebViewEvent* event);
    void HandleWebsocketEvent(const alt::CWebSocketClientEvent* event);
#endif

    // Creates a new timer
    uint32_t CreateTimer(uint32_t timeout, asIScriptFunction* callback, asIScriptObject* object, bool once)
    {
        uint32_t id = nextTimerId++;
        timers[id]  = new Helpers::Timer{ this, callback, object, timeout, GetTime(), once };

        return id;
    }
    void RemoveTimer(uint32_t id)
    {
        invalidTimers.emplace_back(id);
    }

    // Object data
    void                  SetObjectData(alt::IBaseObject* object, const std::string& key, int type, void* value);
    bool                  HasObjectData(alt::IBaseObject* object, const std::string& key);
    std::pair<int, void*> GetObjectData(alt::IBaseObject* object, const std::string& key);
    void                  DeleteObjectData(alt::IBaseObject* object, const std::string& key);

    bool Eval(const std::string& code);

    // Yoinked from v8 helpers
    int64_t GetTime() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    // Shows some debug info about the resource
    // Only used by the debug console command
    void ShowDebugInfo() const;

    bool Start();
    bool Stop();

    bool OnEvent(const alt::CEvent* event);
    void OnTick();

    void OnCreateBaseObject(alt::Ref<alt::IBaseObject> object) override;
    void OnRemoveBaseObject(alt::Ref<alt::IBaseObject> object) override;
};
