#pragma once
#include "Log.h"
#include "../../helpers/module.h"
#include "worldObject.h"
#include "../../helpers/angelscript.h"

static std::string ToString(alt::IColShape* shape)
{
    std::stringstream str;
    str << "ColShape{ type: " << (uint8_t)shape->GetColshapeType() << " }";
    return str.str();
}

static bool IsPointIn(alt::IColShape* shape, Data::Vector3 point)
{
    return shape->IsPointIn({ point.x, point.y, point.z });
}

namespace Helpers
{
    template<class T>
    static void RegisterAsColshape(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsWorldObject<T>(engine, docs, type);

        REGISTER_METHOD_WRAPPER(type, "string opImplConv() const", ToString);

        REGISTER_PROPERTY_WRAPPER_GET(
          type, "int8", "shapeType", (GenericWrapper<T, alt::IColShape, &alt::IColShape::GetColshapeType, alt::IColShape::ColShapeType>));

        REGISTER_PROPERTY_WRAPPER_GET(type, "bool", "playersOnly", (GenericWrapper<T, alt::IColShape, &alt::IColShape::IsPlayersOnly, bool>));
        REGISTER_PROPERTY_WRAPPER_SET(type, "bool", "playersOnly", (GenericWrapper<T, alt::IColShape, &alt::IColShape::SetPlayersOnly, void, bool>));

        REGISTER_METHOD_WRAPPER(type, "bool IsPointIn(Vector3 point) const", IsPointIn);
        REGISTER_METHOD_WRAPPER(
          type, "bool IsEntityIn(Entity@ entity) const", (GenericWrapper<T, alt::IColShape, &alt::IColShape::IsEntityIn, bool, alt::IEntity*>));
    }
}  // namespace Helpers
