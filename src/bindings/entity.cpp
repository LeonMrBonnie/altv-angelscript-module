#include "entity.h"

static ModuleExtension entityExtension("alt", 3, [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_REF_CLASS("Entity", alt::IEntity, asOBJ_REF, "Entity superclass for all alt:V entities");
    RegisterAsEntity<alt::IEntity>(engine, docs, "Entity");
});
