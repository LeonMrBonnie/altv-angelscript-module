#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "helpers/docs.h"

class AngelScriptResource;
class AngelScriptRuntime : public alt::IScriptRuntime
{
    asIScriptEngine* engine;

public:
    AngelScriptRuntime();
    alt::IResource::Impl* CreateImpl(alt::IResource* resource) override;
    void DestroyImpl(alt::IResource::Impl* impl) override;

    asIScriptEngine* GetEngine()
    {
        return engine;
    }

    // Register the script interfaces (the scripting api)
    void RegisterScriptInterfaces(asIScriptEngine* engine, Helpers::DocsGenerator* docs);

    // Gets the current runtime instance or creates one if not exists
    static AngelScriptRuntime& Instance()
	{
		static AngelScriptRuntime _instance;
		return _instance;
	}
};