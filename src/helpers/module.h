#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../resource.h"

#define GLOBAL_FUNC(decl, func, desc) \
    { \
        auto r = engine->RegisterGlobalFunction(decl, asFUNCTION(func), asCALL_CDECL); \
        if(r < 0) \
        { \
            Log::Error << "Failed to register global function '" decl << "'" << Log::Endl; \
        } \
    }

#define FUNCDEF(decl) \
    { \
        auto r = engine->RegisterFuncdef(decl); \
        if(r < 0) \
        { \
            Log::Error << "Failed to register func def '" decl << "'" << Log::Endl; \
        } \
    }

#define GET_RESOURCE() \
    auto resource = static_cast<AngelScriptResource*>(asGetActiveContext()->GetUserData());

namespace Helpers
{
    class ModuleExtension
    {
        static std::vector<ModuleExtension*> extensions;

        using CreateCallback = void(*)(asIScriptEngine*);

        std::string name;
        CreateCallback callback;
    public:
        ModuleExtension(std::string name, CreateCallback callback) : name(name), callback(callback)
        {
            extensions.emplace_back(this);
        }

        std::string GetName()
        {
            return name;
        }

        void Register(asIScriptEngine* engine)
        {
            callback(engine);
        }

        static void RegisterAll(std::string name, asIScriptEngine* engine)
        {
            engine->SetDefaultNamespace(name.c_str());
            for(auto& extension : extensions)
            {
                if(extension->GetName() == name) extension->Register(engine);
            }
        }
    };

    static void MessageHandler(const asSMessageInfo *msg, void *param)
    {
        if(msg->type == asMSGTYPE_INFORMATION) Log::Info << msg->section << " (" << std::to_string(msg->row) << ", " << std::to_string(msg->col) << "): " << msg->message << Log::Endl;
        else if(msg->type == asMSGTYPE_WARNING) Log::Error << msg->section << " (" << std::to_string(msg->row) << ", " << std::to_string(msg->col) << "): " << msg->message << Log::Endl;
        else Log::Warning << msg->section << " (" << std::to_string(msg->row) << ", " << std::to_string(msg->col) << "): " << msg->message << Log::Endl;
    }
}