#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../resource.h"
#include "docs.h"
#include "module.h"

// Registers a new event handler and creates a wrapper function for registering it
#define REGISTER_EVENT_HANDLER(type, name, returnType, decl, argsGetter)                                                   \
    static void On##name(asIScriptFunction* callback)                                                                      \
    {                                                                                                                      \
        GET_RESOURCE();                                                                                                    \
        resource->RegisterEventHandler(type, callback);                                                                    \
    }                                                                                                                      \
    static Event Event##name(type, #name, returnType, decl, argsGetter, [](asIScriptEngine* engine, DocsGenerator* docs) { \
        std::stringstream funcDef;                                                                                         \
        funcDef << returnType << " " << #name << "Callback(" << decl << ")";                                               \
        engine->RegisterFuncdef(funcDef.str().c_str());                                                                    \
        std::stringstream globalFunc;                                                                                      \
        globalFunc << "void On" << #name << "(" << #name << "Callback@ callback)";                                         \
        engine->RegisterGlobalFunction(globalFunc.str().c_str(), asFUNCTION(On##name), asCALL_CDECL);                      \
        docs->PushEventDeclaration(funcDef.str(), globalFunc.str());                                                       \
    });

namespace Helpers
{
    using CallbacksGetter  = std::vector<asIScriptFunction*> (*)(AngelScriptResource* resource, const alt::CEvent* event, std::string name);
    using ExecuteCallback  = int (*)(AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context);
    using RegisterCallback = void (*)(asIScriptEngine* engine, DocsGenerator* docs);

    class Event
    {
        static std::unordered_map<alt::CEvent::Type, Event*> all;

        std::string       name;
        alt::CEvent::Type type;
        const char*       callbackDecl;
        const char*       returnType;
        ExecuteCallback   executeCallback;
        RegisterCallback  registerCallback;

    public:
        Event(alt::CEvent::Type type,
              std::string       name,
              const char*       returnType,
              const char*       callbackDecl,
              ExecuteCallback   executeCallback,
              RegisterCallback  registerCallback)
            : type(type),
              name(std::move(name)),
              callbackDecl(callbackDecl),
              returnType(returnType),
              executeCallback(executeCallback),
              registerCallback(registerCallback)
        {
            all.insert({ type, this });
        };

        int Execute(AngelScriptResource* resource, const alt::CEvent* event)
        {
            return executeCallback(resource, event, resource->GetContext());
        }

        const char* GetReturnType()
        {
            return returnType;
        }

        alt::CEvent::Type GetType()
        {
            return type;
        }

        std::string GetName()
        {
            return name;
        }

        static Event* GetEvent(alt::CEvent::Type type)
        {
            auto event = all.find(type);
            if(event == all.end()) return nullptr;
            return event->second;
        }

        static Event* GetByMetadata(std::string metadata)
        {
            for(auto [_, event] : all)
            {
                if("On" + event->name == metadata) return event;
            }
            return nullptr;
        }

        static void RegisterAll(asIScriptEngine* engine, DocsGenerator* docs)
        {
            for(auto event : all)
            {
                event.second->registerCallback(engine, docs);
            }
        }
    };
}  // namespace Helpers
