#include "baseObject.h"

static ModuleExtension baseObjectExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsBaseObject<alt::IBaseObject>(engine, docs, "BaseObject");
});
