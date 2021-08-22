#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"

static ModuleExtension dictExtension("", [](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptDictionary(engine);
});
