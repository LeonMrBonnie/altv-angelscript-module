#include "baseobject.h"

static ModuleExtension baseObjectExtension("alt", 1, [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_REF_CLASS("BaseObject", alt::IBaseObject, asOBJ_REF, "Base object superclass for all alt:V base objects");
    RegisterAsBaseObject<alt::IBaseObject>(engine, docs, "BaseObject");
});
