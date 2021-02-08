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

    void RegisterScriptInterfaces(asIScriptEngine* engine, Helpers::DocsGenerator* docs);

    static AngelScriptRuntime& Instance()
	{
		static AngelScriptRuntime _instance;
		return _instance;
	}
};