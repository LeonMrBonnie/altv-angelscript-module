#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

#ifdef SERVER_MODULE
static DataExtension propsExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_VALUE_CLASS("Prop", alt::Prop, asOBJ_VALUE | asOBJ_POD, "A prop component");
    REGISTER_PROPERTY("Prop", "const uint16 drawable", alt::Prop, drawableId);
    REGISTER_PROPERTY("Prop", "const uint8 texture", alt::Prop, textureId);

    REGISTER_VALUE_CLASS("DlcProp", alt::DlcProp, asOBJ_VALUE | asOBJ_POD, "A dlc prop component");
    REGISTER_PROPERTY("DlcProp", "const uint16 drawable", alt::DlcProp, drawableId);
    REGISTER_PROPERTY("DlcProp", "const uint8 texture", alt::DlcProp, textureId);
    REGISTER_PROPERTY("DlcProp", "const uint dlc", alt::DlcProp, dlc);
});
#endif
