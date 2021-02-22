#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

namespace Data
{
    static void RegisterProps(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("Prop", alt::Prop, asOBJ_VALUE | asOBJ_POD, "A prop component");
        REGISTER_PROPERTY("Prop", "uint16 drawable", alt::Prop, drawableId);
        REGISTER_PROPERTY("Prop", "uint8 texture", alt::Prop, textureId);

        REGISTER_VALUE_CLASS("DlcProp", alt::DlcProp, asOBJ_VALUE | asOBJ_POD, "A dlc prop component");
        REGISTER_PROPERTY("DlcProp", "uint16 drawable", alt::DlcProp, drawableId);
        REGISTER_PROPERTY("DlcProp", "uint8 texture", alt::DlcProp, textureId);
        REGISTER_PROPERTY("DlcProp", "uint dlc", alt::DlcProp, dlc);
    }
}
