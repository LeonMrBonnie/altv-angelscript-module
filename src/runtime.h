#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "helpers/docs.h"

class AngelScriptResource;
class AngelScriptRuntime : public alt::IScriptRuntime
{
    asIScriptEngine* engine;

    std::unordered_map<alt::IResource*, AngelScriptResource*> resources;

    // Types
    asITypeInfo* arrayStringTypeInfo = nullptr;
    asITypeInfo* arrayIntTypeInfo    = nullptr;
    asITypeInfo* arrayUintTypeInfo   = nullptr;
    asITypeInfo* arrayByteTypeInfo   = nullptr;

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

    CScriptArray* CreateStringArray(uint32_t len);
    CScriptArray* CreateIntArray(uint32_t len);
    CScriptArray* CreateUIntArray(uint32_t len);
    CScriptArray* CreateByteArray(uint8_t* data);
    void          RegisterTypeInfos();
    // Register the script interfaces (the scripting api)
    void RegisterScriptInterfaces(asIScriptEngine* engine);

    const int GetStringTypeId() const
    {
        static int stringType = engine->GetTypeIdByDecl("string");
        return stringType;
    }
    int GetBaseObjectTypeId()
    {
        static int baseObjectType = engine->GetTypeIdByDecl("alt::BaseObject@");
        return baseObjectType;
    }
    int GetWorldObjectTypeId()
    {
        static int worldObjectType = engine->GetTypeIdByDecl("alt::WorldObject@");
        return worldObjectType;
    }
    int GetEntityTypeId()
    {
        static int entityType = engine->GetTypeIdByDecl("alt::Entity@");
        return entityType;
    }
    int GetPlayerTypeId()
    {
        static int playerType = engine->GetTypeIdByDecl("alt::Player@");
        return playerType;
    }
    int GetVehicleTypeId()
    {
        static int vehicleType = engine->GetTypeIdByDecl("alt::Vehicle@");
        return vehicleType;
    }
    int GetVector3TypeId()
    {
        static int vector3Type = engine->GetTypeIdByDecl("alt::Vector3");
        return vector3Type;
    }
    int GetVector2TypeId()
    {
        static int vector2Type = engine->GetTypeIdByDecl("alt::Vector2");
        return vector2Type;
    }
    int GetRGBATypeId()
    {
        static int rgbaType = engine->GetTypeIdByDecl("alt::RGBA");
        return rgbaType;
    }
    int GetDictTypeId()
    {
        static int dictType = engine->GetTypeIdByDecl("dictionary");
        return dictType;
    }
    int GetByteArrayTypeId()
    {
        return arrayByteTypeInfo->GetTypeId();
    }
    int GetUintArrayTypeId()
    {
        return arrayUintTypeInfo->GetTypeId();
    }
    int GetIntArrayTypeId()
    {
        return arrayIntTypeInfo->GetTypeId();
    }
    int GetStringArrayTypeId()
    {
        return arrayStringTypeInfo->GetTypeId();
    }

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
