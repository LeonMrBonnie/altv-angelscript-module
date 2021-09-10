#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "helpers/docs.h"
#include "helpers/typeInfoCache.h"

class AngelScriptResource;
class AngelScriptRuntime : public alt::IScriptRuntime
{
    asIScriptEngine*                                          engine;
    std::unordered_map<alt::IResource*, AngelScriptResource*> resources;
    Helpers::TypeInfoCache                                    typeInfoCache;

    void RegisterTypeInfos();
    // Register the script interfaces (the scripting api)
    void RegisterScriptInterfaces(asIScriptEngine* engine);

public:
    AngelScriptRuntime();
    AngelScriptRuntime(const AngelScriptRuntime&) = delete;
    AngelScriptRuntime(AngelScriptRuntime&&)      = delete;
    AngelScriptRuntime& operator=(const AngelScriptRuntime&) = delete;
    AngelScriptRuntime& operator=(AngelScriptRuntime&&) = delete;

    alt::IResource::Impl* CreateImpl(alt::IResource* resource) override;
    void                  DestroyImpl(alt::IResource::Impl* impl) override;
    void                  OnTick() override;

    asIScriptEngine* GetEngine()
    {
        return engine;
    }

    Helpers::TypeInfoCache& GetTypeInfoCache()
    {
        return typeInfoCache;
    }

    CScriptArray* CreateStringArray(uint32_t len);
    CScriptArray* CreateIntArray(uint32_t len);
    CScriptArray* CreateUIntArray(uint32_t len);
    CScriptArray* CreateByteArray(uint8_t* data);

    AngelScriptResource* GetResource(alt::IResource* resource)
    {
        if(!resources.count(resource)) return nullptr;
        return resources.at(resource);
    }
    AngelScriptResource* GetResourceByModule(asIScriptModule* module)
    {
        for(auto [_, resource] : resources)
        {
            if(resource->GetModule() == module) return resource;
        }
        return nullptr;
    }

    void ShowDebugInfo();

    // Gets the current runtime instance or creates one if not exists
    static AngelScriptRuntime& Instance()
    {
        static AngelScriptRuntime _instance;
        return _instance;
    }
};
