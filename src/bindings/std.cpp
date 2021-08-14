#include "Log.h"
#include "../helpers/module.h"
#include "../helpers/angelscript.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/addon/scriptstdstring/scriptstdstring.h"
#include "angelscript/addon/scripthelper/scripthelper.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptmath/scriptmath.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "angelscript/addon/datetime/datetime.h"
#include "angelscript/addon/scripthandle/scripthandle.h"

using namespace Helpers;

static void ArrayPush(CScriptArray* array, void* item)
{
    array->InsertLast(item);
}
static void* ArrayPop(CScriptArray* array)
{
    void* item = array->At(array->GetSize() - 1);
    if(item == NULL) return NULL;
    array->RemoveLast();
    return item;
}
static void* ArrayShift(CScriptArray* array)
{
    void* item = array->At(0);
    if(item == NULL) return NULL;
    auto  size = Helpers::GetTypeSize(array->GetElementTypeId());
    void* copy = asAllocMem(size);
    memcpy(copy, item, size);
    array->RemoveAt(0);
    return copy;
}

static ModuleExtension stdExtension("", [](asIScriptEngine* engine, DocsGenerator* docs) {
    // *** Array
    RegisterScriptArray(engine, true);
    // Alias for 'insertLast'
    REGISTER_METHOD_WRAPPER("array<T>", "void push(const T&in value)", ArrayPush);
    // Returns last item and removes it from array
    REGISTER_METHOD_WRAPPER("array<T>", "T& pop()", ArrayPop);
    // Returns first item and removes it from array
    REGISTER_METHOD_WRAPPER("array<T>", "T& shift()", ArrayShift);

    // *** String
    RegisterStdString(engine);
    RegisterStdStringUtils(engine);
    // *** Dict
    RegisterScriptDictionary(engine);
    // *** Math
    RegisterScriptMath(engine);
    // *** Any
    RegisterScriptAny(engine);
    // *** Dates & Times
    RegisterScriptDateTime(engine);
    // *** Exceptions
    RegisterExceptionRoutines(engine);
    // *** Generic handle
    RegisterScriptHandle(engine);
});
