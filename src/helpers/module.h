#pragma once
#include <algorithm>
#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "angelscript/addon/scriptbuilder/scriptbuilder.h"
#include "../resource.h"
#include "./docs.h"

// Registers a new global function (e.g. 'alt::Log')
#define REGISTER_GLOBAL_FUNC(decl, func, desc)                                                \
    {                                                                                         \
        auto r = engine->RegisterGlobalFunction(decl, asFUNCTION(func), asCALL_CDECL);        \
        if(r < 0)                                                                             \
        {                                                                                     \
            Log::Error << "Failed to register global function '" << decl << "'" << Log::Endl; \
        }                                                                                     \
        else                                                                                  \
        {                                                                                     \
            docs->PushDeclaration(decl, desc);                                                \
        }                                                                                     \
    }

// Registers a new function definition
#define REGISTER_FUNCDEF(decl, desc)                                                  \
    {                                                                                 \
        auto r = engine->RegisterFuncdef(decl);                                       \
        if(r < 0)                                                                     \
        {                                                                             \
            Log::Error << "Failed to register funcdef '" << decl << "'" << Log::Endl; \
        }                                                                             \
        else                                                                          \
        {                                                                             \
            docs->PushFuncDef(decl, desc);                                            \
        }                                                                             \
    }

// Registers a new type definition
#define REGISTER_TYPEDEF(type, original)                                              \
    {                                                                                 \
        auto r = engine->RegisterTypedef(type, original);                             \
        if(r < 0)                                                                     \
        {                                                                             \
            Log::Error << "Failed to register typedef '" << type << "'" << Log::Endl; \
        }                                                                             \
        else                                                                          \
        {                                                                             \
            docs->PushTypedef(type, original);                                        \
        }                                                                             \
    }

// Registers a new value type class (e.g. Vector3)
#define REGISTER_VALUE_CLASS(name, type, flags, desc)                                    \
    {                                                                                    \
        engine->RegisterObjectType(name, sizeof(type), flags | asGetTypeTraits<type>()); \
        docs->PushObjectType(name, desc, "");                                            \
    }

// Registers a new ref type class (e.g. Player)
#define REGISTER_REF_CLASS(name, type, flags, desc, base) \
    {                                                     \
        engine->RegisterObjectType(name, 0, flags);       \
        docs.PushObjectType(name, desc, "" base);         \
    }

// Registers a new class constructor
#define REGISTER_CONSTRUCTOR(name, decl, func)                                                                                  \
    {                                                                                                                           \
        engine->RegisterObjectBehaviour(name, asBEHAVE_CONSTRUCT, "void f(" decl ")", asFUNCTION(func), asCALL_CDECL_OBJFIRST); \
        docs->PushObjectConstructor(name, decl);                                                                                \
    }

// Registers a new class destructor
#define REGISTER_DESTRUCTOR(name, func)                                                                                \
    {                                                                                                                  \
        engine->RegisterObjectBehaviour(name, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(func), asCALL_CDECL_OBJFIRST); \
    }

// Registers a new class factory (only used for classes that have a constructor in the scripting api)
#define REGISTER_FACTORY(name, decl, func)                                                                             \
    {                                                                                                                  \
        engine->RegisterObjectBehaviour(name, asBEHAVE_FACTORY, name "@ f(" decl ")", asFUNCTION(func), asCALL_CDECL); \
        docs->PushObjectConstructor(name, decl);                                                                       \
    }

// Registers a new initializer list constructor
#define REGISTER_INIT_LIST_CONSTRUCTOR(name, decl, func)                                                                                            \
    {                                                                                                                                               \
        engine->RegisterObjectBehaviour(name, asBEHAVE_LIST_CONSTRUCT, "void f(const int&in) {" decl "}", asFUNCTION(func), asCALL_CDECL_OBJFIRST); \
    }

// Registers a new property for the value type class
#define REGISTER_PROPERTY(name, decl, class, property)                         \
    {                                                                          \
        engine->RegisterObjectProperty(name, decl, asOFFSET(class, property)); \
        docs->PushObjectProperty(name, decl);                                  \
    }

// Registers a new method for the value type class
#define REGISTER_METHOD(name, decl, class, method)                                          \
    {                                                                                       \
        engine->RegisterObjectMethod(name, decl, asMETHOD(class, method), asCALL_THISCALL); \
        docs->PushObjectMethod(name, decl);                                                 \
    }

// Registers a new method with a wrapper for the ref type class
#define REGISTER_METHOD_WRAPPER(name, decl, wrapperFn)                                          \
    {                                                                                           \
        engine->RegisterObjectMethod(name, decl, asFUNCTION(wrapperFn), asCALL_CDECL_OBJFIRST); \
        docs->PushObjectMethod(name, decl);                                                     \
    }

// Registers a new property getter wrapper for the class
#define REGISTER_PROPERTY_WRAPPER_GET(name, type, prop, getFn)                                                               \
    {                                                                                                                        \
        engine->RegisterObjectMethod(name, type " get_" prop "() const property", asFUNCTION(getFn), asCALL_CDECL_OBJFIRST); \
        docs->PushObjectProperty(name, type " " prop);                                                                       \
    }

// Registers a new property setter wrapper for the class
#define REGISTER_PROPERTY_WRAPPER_SET(name, type, prop, setFn)                                                                \
    {                                                                                                                         \
        engine->RegisterObjectMethod(name, "void set_" prop "(" type ") property", asFUNCTION(setFn), asCALL_CDECL_OBJFIRST); \
    }

// Registers a global property (e.g. 'alt::resourceName')
#define REGISTER_GLOBAL_PROPERTY_READ_ONLY(type, prop, getFn)                                             \
    {                                                                                                     \
        engine->RegisterGlobalFunction(type " get_" prop "() property", asFUNCTION(getFn), asCALL_CDECL); \
        docs->PushVariable(type, prop);                                                                   \
    }
// Registers a global property (e.g. 'alt::resourceName')
#define REGISTER_GLOBAL_PROPERTY(type, prop, getFn, setFn)                                                       \
    {                                                                                                            \
        engine->RegisterGlobalFunction(type " get_" prop "() property", asFUNCTION(getFn), asCALL_CDECL);        \
        engine->RegisterGlobalFunction("void set_" prop "(" type ") property", asFUNCTION(setFn), asCALL_CDECL); \
        docs->PushVariable(type, prop);                                                                          \
    }

// Registers a new global enum
#define REGISTER_ENUM(name, desc)       \
    {                                   \
        engine->RegisterEnum(name);     \
        docs->PushEnumType(name, desc); \
    }

// Registers a new value for the specified enum
#define REGISTER_ENUM_VALUE(enum, name, value)                 \
    {                                                          \
        engine->RegisterEnumValue(enum, name, (uint8_t)value); \
        docs->PushEnumValue(enum, name, (uint8_t)value);       \
    }

#define REGISTER_VARIADIC_FUNC(type, name, defaultArgs, argCount, func, desc)                               \
    {                                                                                                       \
        std::stringstream stream;                                                                           \
        stream << type << " " << name << "(" << defaultArgs;                                                \
        engine->RegisterGlobalFunction((stream.str() + ")").c_str(), asFUNCTION(func), asCALL_GENERIC);     \
        for(int i = 0; i < argCount; i++)                                                                   \
        {                                                                                                   \
            stream << ", ?&in arg" + std::to_string(i);                                                     \
            engine->RegisterGlobalFunction((stream.str() + ")").c_str(), asFUNCTION(func), asCALL_GENERIC); \
        }                                                                                                   \
        docs->PushDeclaration(type " " name "(" defaultArgs ", ...)", desc);                                \
    }

#define REGISTER_VARIADIC_METHOD(class, type, name, defaultArgs, argCount, func)                                 \
    {                                                                                                            \
        std::stringstream stream;                                                                                \
        stream << type << " " << name << "(" << defaultArgs;                                                     \
        engine->RegisterObjectMethod(class, (stream.str() + ")").c_str(), asFUNCTION(func), asCALL_GENERIC);     \
        for(int i = 0; i < argCount; i++)                                                                        \
        {                                                                                                        \
            stream << ", ?&in arg" + std::to_string(i);                                                          \
            engine->RegisterObjectMethod(class, (stream.str() + ")").c_str(), asFUNCTION(func), asCALL_GENERIC); \
        }                                                                                                        \
        docs->PushObjectMethod(class, type " " name "(" defaultArgs ", ...)");                                   \
    }

// Gets the currently active resource
#define GET_RESOURCE()  auto resource = static_cast<AngelScriptResource*>(asGetActiveContext()->GetUserData())
#define GET_IRESOURCE() auto resource = static_cast<AngelScriptResource*>(asGetActiveContext()->GetUserData())->GetIResource()

// Throws a script error
#define THROW_ERROR(error) asGetActiveContext()->SetException(error)

// Checks the return from the angelscript function and if necessary logs an error
#define CHECK_AS_RETURN(type, result, returnOnError)                                         \
    if(result < 0)                                                                           \
    {                                                                                        \
        Log::Error << type << " error. Error code: " << std::to_string(result) << Log::Endl; \
        return returnOnError;                                                                \
    }

namespace Helpers
{
    using CreateCallback = void (*)(asIScriptEngine*, DocsGenerator*);
    class ModuleExtension
    {
        static std::vector<ModuleExtension*> extensions;

        std::string    name;
        CreateCallback callback;
        bool           isDataExtension;

    public:
        // Creates a new module extension
        // Module extensions are used to register new classes, properties, methods etc.
        ModuleExtension(std::string name, CreateCallback callback, bool isDataExtension = false)
            : name(std::move(name)), callback(callback), isDataExtension(isDataExtension)
        {
            extensions.push_back(this);
        }

        std::string GetName()
        {
            return name;
        }
        bool IsDataExtension()
        {
            return isDataExtension;
        }

        void Register(asIScriptEngine* engine, DocsGenerator* docs)
        {
            callback(engine, docs);
        }

        // Registers all module extensions for the given module
        static void RegisterAll(std::string name, asIScriptEngine* engine, DocsGenerator* docs = nullptr)
        {
            // Create docs generator if none was passed
            bool cleanupDocs = false;
            if(docs == nullptr)
            {
                cleanupDocs = true;
                docs        = new DocsGenerator(name);
            }
            // Sets the namespace to the module name
            engine->SetDefaultNamespace(name.c_str());
            for(auto extension : extensions)
            {
                if(!extension->IsDataExtension() && extension->GetName() == name) extension->Register(engine, docs);
            }
            docs->Generate();
            if(cleanupDocs) delete docs;
        }
        // Registers all data extensions
        static void RegisterAllData(asIScriptEngine* engine, DocsGenerator* docs)
        {
            engine->SetDefaultNamespace("alt");
            for(auto extension : extensions)
            {
                if(extension->IsDataExtension()) extension->Register(engine, docs);
            }
        }

        static size_t GetCount()
        {
            std::vector<std::string> counted;
            for(auto extension : extensions)
            {
                if(std::find(counted.begin(), counted.end(), extension->GetName()) == counted.end()) counted.push_back(extension->GetName());
            }
            return counted.size();
        }
    };
    class DataExtension : public ModuleExtension
    {
    public:
        DataExtension(CreateCallback callback) : ModuleExtension("alt", callback, true) {}
    };

    // Generic Wrapper for class methods
    template<typename Type, typename OriginalType, auto Func, typename ReturnType = void, typename... Args>
    static ReturnType GenericWrapper(Type* c, Args... args)
    {
        return std::invoke(Func, dynamic_cast<OriginalType*>(c), args...);
    }
}  // namespace Helpers
