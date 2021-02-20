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

namespace Helpers
{
    static void RegisterAsColshape(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        RegisterAsWorldObject<alt::IColShape>(engine, docs, type);

        REGISTER_METHOD_WRAPPER(type, "string opImplConv() const", ToString);

        REGISTER_PROPERTY_WRAPPER_GET(type, "int8", "type", (GenericWrapper<alt::IColShape, alt::IColShape, &alt::IColShape::GetColshapeType, alt::IColShape::ColShapeType>));
    }
}

