#pragma once
#include "Log.h"
#include "../../helpers/module.h"
#include "baseObject.h"
#include "../data/vector3.h"

template<class T>
static Data::Vector3 GetPosition(T* obj)
{
    alt::Vector3f pos = obj->GetPosition();
    return Data::Vector3(pos[0], pos[1], pos[2]);
}

template<class T>
static void SetPosition(T* obj, Data::Vector3 pos)
{
    obj->SetPosition(alt::Position{ pos.x, pos.y, pos.z });
}

using namespace Helpers;

namespace Helpers
{
    template<class T>
    static void RegisterAsWorldObject(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsBaseObject<T>(engine, docs, type);

        REGISTER_PROPERTY_WRAPPER_GET(type, "Vector3", "pos", GetPosition<T>);
        REGISTER_PROPERTY_WRAPPER_SET(type, "Vector3", "pos", SetPosition<T>);

        REGISTER_PROPERTY_WRAPPER_GET(type, "int", "dimension", (GenericWrapper<T, alt::IWorldObject, &alt::IWorldObject::GetDimension, int32_t>));
        REGISTER_PROPERTY_WRAPPER_SET(
          type, "int", "dimension", (GenericWrapper<T, alt::IWorldObject, &alt::IWorldObject::SetDimension, void, int32_t>));
    }
}  // namespace Helpers
