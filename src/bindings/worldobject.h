#pragma once
#include "Log.h"
#include "../helpers/module.h"
#include "baseobject.h"
#include "vector3.h"

template<class T>
static Helpers::Vector3<float>* GetPosition(T* obj)
{
    auto pos = obj->GetPosition();
    return new Helpers::Vector3<float>(pos.x, pos.y, pos.z);
}

template<class T>
static void SetPosition(float x, float y, float z, T* obj)
{
    obj->SetPosition(alt::Point{x, y, z});
}

template<class T>
static int GetDimension(T* obj)
{
    return obj->GetDimension();
}

template<class T>
static void SetDimension(int dimension, T* obj)
{
    obj->SetDimension(dimension);
}

using namespace Helpers;

namespace Helpers
{
    template<class T>
    static void RegisterAsWorldObject(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsBaseObject<T>(engine, docs, type);

        // todo: fix this
        /*
        REGISTER_PROPERTY_WRAPPER_GET(type, "Vector3f", "position", GetPosition<T>);
        REGISTER_METHOD_WRAPPER(type, "Vector3f GetPosition()", GetPosition<T>);
        REGISTER_METHOD_WRAPPER(type, "void SetPosition(float x, float y, float z)", SetPosition<T>);
        */

        REGISTER_PROPERTY_WRAPPER_GET(type, "int", "dimension", GetDimension<T>);
        REGISTER_PROPERTY_WRAPPER_SET(type, "int", "dimension", SetDimension<T>);
    }
}
