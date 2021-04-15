#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

static ModuleExtension weatherEnum("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    std::vector<const char*> weathers = {
        "EXTRA_SUNNY",
        "CLEAR",
        "CLOUDS",
        "SMOG",
        "FOGGY",
        "OVERCAST",
        "RAIN",
        "THUNDER",
        "CLEARING",
        "NEUTRAL",
        "SNOW",
        "BLIZZARD",
        "SNOWLIGHT",
        "XMAS",
        "HALLOWEEN"
    };

    REGISTER_ENUM("WeatherType", "GTA weather types");
    for(uint32_t i = 0; i < weathers.size(); i++)
    {
        REGISTER_ENUM_VALUE("WeatherType", weathers[i], i);
    }
});
