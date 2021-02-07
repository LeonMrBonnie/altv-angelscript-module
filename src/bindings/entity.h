#pragma once
#include "Log.h"
#include "../helpers/module.h"
#include "worldobject.h"

using namespace Helpers;

template<class T>
static uint32_t GetID(T* obj)
{
    return obj->GetID();
}

template<class T>
static uint32_t GetModel(T* obj)
{
    return obj->GetModel();
}

namespace Helpers
{
    template<class T>
    static void RegisterAsEntity(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsWorldObject<T>(engine, docs, type);

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "id", GetID<T>);

        REGISTER_PROPERTY_WRAPPER_GET(type, "uint", "model", GetModel<T>);

        // todo: add missing methods
    }
}
