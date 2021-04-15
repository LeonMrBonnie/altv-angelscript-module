#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

static ModuleExtension baseObjectTypeEnum("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_ENUM("BaseObjectType", "Types used by base objects");
    REGISTER_ENUM_VALUE("BaseObjectType", "PLAYER", alt::IBaseObject::Type::PLAYER);
    REGISTER_ENUM_VALUE("BaseObjectType", "VEHICLE", alt::IBaseObject::Type::VEHICLE);
    REGISTER_ENUM_VALUE("BaseObjectType", "BLIP", alt::IBaseObject::Type::BLIP);
    REGISTER_ENUM_VALUE("BaseObjectType", "WEBVIEW", alt::IBaseObject::Type::WEBVIEW);
    REGISTER_ENUM_VALUE("BaseObjectType", "VOICE_CHANNEL", alt::IBaseObject::Type::VOICE_CHANNEL);
    REGISTER_ENUM_VALUE("BaseObjectType", "COLSHAPE", alt::IBaseObject::Type::COLSHAPE);
    REGISTER_ENUM_VALUE("BaseObjectType", "CHECKPOINT", alt::IBaseObject::Type::CHECKPOINT);
    REGISTER_ENUM_VALUE("BaseObjectType", "WEBSOCKET_CLIENT", alt::IBaseObject::Type::WEBSOCKET_CLIENT);
});
