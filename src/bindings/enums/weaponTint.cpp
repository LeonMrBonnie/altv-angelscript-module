#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

static ModuleExtension weaponTintEnum("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    std::vector<const char*> tints = { "NORMAL", "GREEN", "GOLD", "PINK", "ARMY", "LSPD", "ORANGE", "PLATINUM" };

    REGISTER_ENUM("WeaponTint", "GTA weapon tints");
    for(uint32_t i = 0; i < tints.size(); i++)
    {
        REGISTER_ENUM_VALUE("WeaponTint", tints[i], i);
    }
});
