#include "worldobject.h"

static ModuleExtension worldObjectExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_REF_CLASS("WorldObject", alt::IWorldObject, asOBJ_REF, "World object superclass for all alt:V world objects");
    RegisterAsWorldObject<alt::IWorldObject>(engine, docs, "WorldObject");
});
