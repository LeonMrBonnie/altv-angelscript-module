#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../resource.h"
#include "docs.h"
#include "module.h"

namespace Helpers
{
    void OnEvent(std::string& event, asIScriptFunction* callback);

    using CallbacksGetter = std::vector<asIScriptFunction*>(*)(AngelScriptResource* resource, const alt::CEvent* event, std::string name);
    using ArgsGetter = void(*)(AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args);

    class Event
    {
        static std::unordered_map<alt::CEvent::Type, Event*> all;

        std::string eventName;
        const char* callbackDecl;
        ArgsGetter argsGetter;
        CallbacksGetter callbacksGetter;
        bool skipDecl;

    public:
        Event(alt::CEvent::Type type, std::string name, const char* callbackDecl, ArgsGetter argsGetter, CallbacksGetter callbacksGetter, bool skipDecl = false) : 
            eventName(name),
            callbackDecl(callbackDecl),
            argsGetter(argsGetter),
            callbacksGetter(callbacksGetter),
            skipDecl(skipDecl)
        {
            all.insert({type, this});
        };

        std::vector<asIScriptFunction*> GetCallbacks(AngelScriptResource* resource, const alt::CEvent* event)
        {
            return callbacksGetter(resource, event, eventName);
        }

        std::vector<std::pair<void*, bool>> GetArgs(AngelScriptResource* resource, const alt::CEvent* event)
        {
            std::vector<std::pair<void*, bool>> args;
            argsGetter(resource, event, args);
            return args;
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
                std::string uppercased = event.second->eventName;
                uppercased[0] = toupper(uppercased[0]);
                std::string decl;
                decl.append("void ");
                decl.append(uppercased);
                decl.append("Callback(");
                decl.append(event.second->callbackDecl);
                decl.append(")");
                if(!event.second->skipDecl) engine->RegisterFuncdef(decl.c_str());
                docs->PushEventDeclaration(event.second->eventName, decl);
                std::string onEventDecl;
                onEventDecl.append("void on(const string &in event, ");
                onEventDecl.append(uppercased);
                onEventDecl.append("Callback@ callback"); 
                onEventDecl.append(")");
                if(!event.second->skipDecl) REGISTER_GLOBAL_FUNC(onEventDecl.c_str(), &OnEvent, "Registers a event handler");
            }
        }
    };
}