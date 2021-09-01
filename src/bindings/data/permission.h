#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

#ifdef SERVER_MODULE
static DataExtension permissionExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_ENUM("Permission", "A alt:V permission");
    REGISTER_ENUM_VALUE("Permission", "None", alt::Permission::None);
    REGISTER_ENUM_VALUE("Permission", "ScreenCapture", alt::Permission::ScreenCapture);
    REGISTER_ENUM_VALUE("Permission", "WebRTC", alt::Permission::WebRTC);
    REGISTER_ENUM_VALUE("Permission", "All", alt::Permission::All);
    REGISTER_ENUM("PermissionState", "A alt:V permission state");
    REGISTER_ENUM_VALUE("PermissionState", "Allowed", alt::PermissionState::Allowed);
    REGISTER_ENUM_VALUE("PermissionState", "Denied", alt::PermissionState::Denied);
    REGISTER_ENUM_VALUE("PermissionState", "Unspecified", alt::PermissionState::Unspecified);
    REGISTER_ENUM_VALUE("PermissionState", "Failed", alt::PermissionState::Failed);
});
#endif
