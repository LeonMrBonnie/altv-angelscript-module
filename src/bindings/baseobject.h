#pragma once
#include "Log.h"
#include "../helpers/module.h"

using namespace Helpers;

template<class T>
static void AddRef(T* obj)
{
    obj->AddRef();
}

template<class T>
static void RemoveRef(T* obj)
{
    obj->RemoveRef();
}

template<class T>
static uint32_t GetType(T* obj)
{
    return (uint8_t)obj->GetType();
}

namespace Helpers
{
    template<class T>
    static void RegisterAsBaseObject(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        engine->RegisterObjectBehaviour(type, asBEHAVE_ADDREF, "void f()", asFUNCTION(AddRef<T>), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour(type, asBEHAVE_RELEASE, "void f()", asFUNCTION(RemoveRef<T>), asCALL_CDECL_OBJLAST);

        REGISTER_PROPERTY_WRAPPER_GET(type, "int", "type", GetType<T>);
        
        // todo: add meta methods
    }
}
