#include "worldobject.h"

static ModuleExtension worldObjectExtension("alt", 2, [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_REF_CLASS("WorldObject", alt::IWorldObject, asOBJ_REF, "World object superclass for all alt:V world objects");
    RegisterAsWorldObject<alt::IWorldObject>(engine, docs, "WorldObject");
});
