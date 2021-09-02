#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/add_on/scriptdictionary/scriptdictionary.h"

static StdExtension dictExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptDictionary(engine);
});
