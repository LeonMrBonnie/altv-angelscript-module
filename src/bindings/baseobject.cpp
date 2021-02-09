#include "baseobject.h"

static ModuleExtension baseObjectExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsBaseObject<alt::IBaseObject>(engine, docs, "BaseObject");

    REGISTER_ENUM("ObjectType", "Types used by base objects");
    REGISTER_ENUM_VALUE("ObjectType", "PLAYER", alt::IBaseObject::Type::PLAYER);
    REGISTER_ENUM_VALUE("ObjectType", "VEHICLE", alt::IBaseObject::Type::VEHICLE);
    REGISTER_ENUM_VALUE("ObjectType", "BLIP", alt::IBaseObject::Type::BLIP);
    REGISTER_ENUM_VALUE("ObjectType", "WEBVIEW", alt::IBaseObject::Type::WEBVIEW);
    REGISTER_ENUM_VALUE("ObjectType", "VOICE_CHANNEL", alt::IBaseObject::Type::VOICE_CHANNEL);
    REGISTER_ENUM_VALUE("ObjectType", "COLSHAPE", alt::IBaseObject::Type::COLSHAPE);
    REGISTER_ENUM_VALUE("ObjectType", "CHECKPOINT", alt::IBaseObject::Type::CHECKPOINT);
    REGISTER_ENUM_VALUE("ObjectType", "WEBSOCKET_CLIENT", alt::IBaseObject::Type::WEBSOCKET_CLIENT);
});
