#pragma once
#include <algorithm>
#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "angelscript/addon/scriptbuilder/scriptbuilder.h"
#include "../resource.h"
#include "./docs.h"

#define REGISTER_GLOBAL_FUNC(decl, func, desc) \
    { \
        auto r = engine->RegisterGlobalFunction(decl, asFUNCTION(func), asCALL_CDECL); \
        if(r < 0) \
        { \
            Log::Error << "Failed to register global function '" << decl << "'" << Log::Endl; \
        } \
        else \
        { \
            docs->PushDeclaration(decl, desc); \
        } \
    }

#define REGISTER_FUNCDEF(decl, desc) \
    { \
        auto r = engine->RegisterFuncdef(decl); \
        if(r < 0) \
        { \
            Log::Error << "Failed to register func def '" decl << "'" << Log::Endl; \
        } \
        else \
        { \
            docs->PushFuncDef(decl, desc); \
        } \
    }

#define REGISTER_VALUE_CLASS(name, type, flags, desc) \
    { \
        engine->RegisterObjectType(name, sizeof(type), flags | asGetTypeTraits<type>()); \
        docs->PushObjectType(name, desc); \
    }

#define REGISTER_REF_CLASS(name, type, flags, desc) \
    { \
        engine->RegisterObjectType(name, 0, flags); \
        docs->PushObjectType(name, desc); \
    }

#define REGISTER_CONSTRUCTOR(name, decl, func) \
    { \
        engine->RegisterObjectBehaviour(name, asBEHAVE_CONSTRUCT, "void f("##decl##")", asFUNCTION(func), asCALL_CDECL_OBJLAST); \
        docs->PushObjectConstructor(name, decl); \
    }

#define REGISTER_PROPERTY(name, decl, class, property) \
    { \
        engine->RegisterObjectProperty(name, decl, asOFFSET(class, property)); \
        docs->PushObjectProperty(name, decl); \
    }

#define REGISTER_METHOD(name, decl, class, method) \
    { \
        engine->RegisterObjectMethod(name, decl, asMETHOD(class, method), asCALL_THISCALL); \
        docs->PushObjectMethod(name, decl); \
    }

#define REGISTER_METHOD_WRAPPER(name, decl, wrapperFn) \
    { \
        engine->RegisterObjectMethod(name, decl, asFUNCTION(wrapperFn), asCALL_CDECL_OBJLAST); \
        docs->PushObjectMethod(name, decl); \
    }

#define REGISTER_PROPERTY_WRAPPER_GET(name, type, prop, getFn) \
    { \
        engine->RegisterObjectMethod(name, ##type##" get_"##prop##"() const property", asFUNCTION(getFn), asCALL_CDECL_OBJLAST); \
        docs->PushObjectProperty(name, ##type##" "##prop##); \
    }

#define REGISTER_PROPERTY_WRAPPER_SET(name, type, prop, setFn) \
    { \
        engine->RegisterObjectMethod(name, "void set_"##prop##"("##type##") property", asFUNCTION(setFn), asCALL_CDECL_OBJLAST); \
    }

#define GET_RESOURCE() \
    auto resource = static_cast<AngelScriptResource*>(asGetActiveContext()->GetUserData())

#define CHECK_AS_RETURN(type, result, returnOnError) \
    if(r < 0) \
    { \
        Log::Error << type << " error. Error code: " << std::to_string(result) << Log::Endl;\
        return returnOnError;\
    }

namespace Helpers
{
    class ModuleExtension
    {
        static std::vector<ModuleExtension*> extensions;

        using CreateCallback = void(*)(asIScriptEngine*, DocsGenerator*);

        std::string name;
        CreateCallback callback;
    public:
        ModuleExtension(std::string name, CreateCallback callback) : name(name), callback(callback)
        {
            extensions.push_back(this);
        }

        std::string GetName()
        {
            return name;
        }

        void Register(asIScriptEngine* engine, DocsGenerator* docs)
        {
            callback(engine, docs);
        }

        static void RegisterAll(std::string name, asIScriptEngine* engine, DocsGenerator* docs)
        {
            engine->SetDefaultNamespace(name.c_str());
            for(auto extension : extensions)
            {
                if(extension->GetName() == name) extension->Register(engine, docs);
            }
        }
    };

    // Handles includes
    static int IncludeHandler(const char* include, const char* from, CScriptBuilder* builder, void* data)
    {
        // todo: add support for relative paths
        auto resource = static_cast<AngelScriptResource*>(data);
        auto src = resource->ReadFile(alt::String(include));
        int r = builder->AddSectionFromMemory(include, src.CStr(), src.GetSize());
        CHECK_AS_RETURN("Include", r, -1);
        return 0;
    }

    // Handles pragma directives
    static int PragmaHandler(const std::string& pragmaText, CScriptBuilder& builder, void* data)
    {
        auto resource = static_cast<AngelScriptResource*>(data);
        return 0;
    }

    static void MessageHandler(const asSMessageInfo *msg, void *param)
    {
        if(msg->type == asMSGTYPE_INFORMATION) Log::Info << msg->section << " (" << std::to_string(msg->row) << ", " << std::to_string(msg->col) << "): " << msg->message << Log::Endl;
        else if(msg->type == asMSGTYPE_WARNING) Log::Error << msg->section << " (" << std::to_string(msg->row) << ", " << std::to_string(msg->col) << "): " << msg->message << Log::Endl;
        else Log::Warning << msg->section << " (" << std::to_string(msg->row) << ", " << std::to_string(msg->col) << "): " << msg->message << Log::Endl;
    }
}