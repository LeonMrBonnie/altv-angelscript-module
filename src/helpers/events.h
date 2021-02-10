#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../resource.h"
#include "docs.h"
#include "module.h"

// Registers a new event handler and creates a wrapper function for registering it
#define REGISTER_EVENT_HANDLER(type, name, returnType, decl, argsGetter) \
    static void On##name##(asIScriptFunction* callback) { \
        GET_RESOURCE(); \
        resource->RegisterEventHandler(type, callback); \
    } \
    static Event Event##name##(type, returnType, decl, argsGetter, [](asIScriptEngine* engine, DocsGenerator* docs) { \
        std::stringstream funcDef; \
        funcDef << returnType" " << #name << "Callback(" << decl << ")"; \
        engine->RegisterFuncdef(funcDef.str().c_str()); \
        std::stringstream globalFunc; \
        globalFunc << "void On" << #name << "(" << #name << "Callback@ callback)"; \
        engine->RegisterGlobalFunction(globalFunc.str().c_str(), asFUNCTION(On##name##), asCALL_CDECL); \
        docs->PushEventDeclaration(funcDef.str(), globalFunc.str()); \
    });

namespace Helpers
{
    using CallbacksGetter = std::vector<asIScriptFunction*>(*)(AngelScriptResource* resource, const alt::CEvent* event, std::string name);
    // args.pair.first = pointer to object, args.pair.second = boolean whether the value is a primitive
    using ArgsGetter = void(*)(AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args);
    using RegisterCallback = void(*)(asIScriptEngine* engine, DocsGenerator* docs);

    class Event
    {
        static std::unordered_map<alt::CEvent::Type, Event*> all;

        const char* callbackDecl;
        const char* returnType;
        ArgsGetter argsGetter;
        RegisterCallback registerCallback;

    public:
        Event(
            alt::CEvent::Type type, 
            const char* returnType,
            const char* callbackDecl, 
            ArgsGetter argsGetter,
            RegisterCallback registerCallback
        ) : 
            callbackDecl(callbackDecl),
            returnType(returnType),
            argsGetter(argsGetter),
            registerCallback(registerCallback)
        {
            all.insert({type, this});
        };

        std::vector<std::pair<void*, bool>> GetArgs(AngelScriptResource* resource, const alt::CEvent* event)
        {
            std::vector<std::pair<void*, bool>> args;
            argsGetter(resource, event, args);
            return args;
        }

        const char* GetReturnType()
        {
            return returnType;
        }

        static Event* GetEvent(alt::CEvent::Type type)
        {
            auto event = all.find(type);
            if(event == all.end()) return nullptr;
            return event->second;
        }

        static void RegisterAll(asIScriptEngine* engine, DocsGenerator* docs)
        {
            for(auto event : all)
            {
                event.second->registerCallback(engine, docs);
            }
        }
    };
}