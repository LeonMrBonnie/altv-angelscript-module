#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

namespace Data
{
    static void RegisterClothes(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("Cloth", alt::Cloth, asOBJ_VALUE | asOBJ_POD, "A clothing component");
        REGISTER_PROPERTY("Cloth", "uint16 drawable", alt::Cloth, drawableId);
        REGISTER_PROPERTY("Cloth", "uint8 texture", alt::Cloth, textureId);
        REGISTER_PROPERTY("Cloth", "uint8 palette", alt::Cloth, paletteId);

        REGISTER_VALUE_CLASS("DlcCloth", alt::DlcCloth, asOBJ_VALUE | asOBJ_POD, "A dlc clothing component");
        REGISTER_PROPERTY("DlcCloth", "uint16 drawable", alt::DlcCloth, drawableId);
        REGISTER_PROPERTY("DlcCloth", "uint8 texture", alt::DlcCloth, textureId);
        REGISTER_PROPERTY("DlcCloth", "uint8 palette", alt::DlcCloth, paletteId);
        REGISTER_PROPERTY("DlcCloth", "uint dlc", alt::DlcCloth, dlc);
    }
}
