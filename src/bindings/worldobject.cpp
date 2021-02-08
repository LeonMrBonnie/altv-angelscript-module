#include "worldobject.h"

static ModuleExtension worldObjectExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsWorldObject<alt::IWorldObject>(engine, docs, "WorldObject");
});
