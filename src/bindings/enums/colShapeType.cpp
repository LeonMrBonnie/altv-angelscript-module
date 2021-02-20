#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

static ModuleExtension colShapeTypeEnum("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_ENUM("ColShapeType", "Types used by colshapes");
    REGISTER_ENUM_VALUE("ColShapeType", "SPHERE", alt::IColShape::ColShapeType::SPHERE);
    REGISTER_ENUM_VALUE("ColShapeType", "CYLINDER", alt::IColShape::ColShapeType::CYLINDER);
    REGISTER_ENUM_VALUE("ColShapeType", "CIRCLE", alt::IColShape::ColShapeType::CIRCLE);
    REGISTER_ENUM_VALUE("ColShapeType", "CUBOID", alt::IColShape::ColShapeType::CUBOID);
    REGISTER_ENUM_VALUE("ColShapeType", "RECT", alt::IColShape::ColShapeType::RECT);
    REGISTER_ENUM_VALUE("ColShapeType", "CHECKPOINT", alt::IColShape::ColShapeType::CHECKPOINT_CYLINDER);
});
