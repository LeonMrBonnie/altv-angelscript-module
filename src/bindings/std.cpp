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

    int  type     = array->GetElementTypeId();
    auto engine   = AngelScriptRuntime::Instance().GetEngine();
    auto typeInfo = engine->GetTypeInfoById(type);

    // Item is an object handle
    if(type & asTYPEID_OBJHANDLE)
    {
        void* result = nullptr;
        engine->RefCastObject(item, typeInfo, typeInfo, &result);
        AS_ASSERT(result, "Failed to ref cast object", nullptr);
        return result;
    }
    // Item is a script object
    else if(type & asTYPEID_SCRIPTOBJECT)
    {
        return engine->CreateScriptObjectCopy(item, typeInfo);
    }
    // Item is a primitive
    else
    {
        auto  size = Helpers::GetTypeSize(type);
        void* copy = asAllocMem(size);
        memcpy(copy, item, size);
        array->RemoveAt(0);
        return copy;
    }
}

static void* ArrayFind(CScriptArray* array, asIScriptFunction* callback)
{
    AS_ASSERT(callback, "Invalid callback specified", NULL);
    asUINT size = array->GetSize();

    auto context = asGetActiveContext();
    for(asUINT i = 0; i < size; i++)
    {
        void* result = CallFunction(context, callback, { { &i, asTYPEID_UINT32 } });
        if(result == nullptr) break;
        if(*(bool*)result) return array->At(i);
    }

    callback->Release();
    return NULL;
}

static void StringStreamDefaultConstructor(void* memory)
{
    new(memory) std::stringstream();
}

static void StringStreamConstructor(void* memory, const std::string& value)
{
    new(memory) std::stringstream(value);
}

static std::string StringStreamToString(std::stringstream* stream)
{
    return stream->str();
}

static std::stringstream* StringStreamPipeIn(std::stringstream* stream, const std::string& value)
{
    *stream << value;
    return stream;
}

static std::stringstream* StringStreamPipeInVarArg(std::stringstream* stream, void* val, int typeId)
{
    *stream << Helpers::ValueToString(val, typeId);
    return stream;
}

static std::stringstream* StringStreamPipeOut(std::stringstream* stream, std::string& value)
{
    value = stream->str();
    return stream;
}

static inline std::string RepeatStr(std::string& str, uint32_t times, const std::string& seperator)
{
    std::string result;
    bool        useSeperator = seperator.size() != 0;
    for(uint32_t i = 0; i < times; i++)
    {
        if(i != 0 && useSeperator) result += seperator;
        result += str;
    }
    return result;
}
static std::string StringRepeat(std::string* str, uint32_t times, const std::string& seperator)
{
    return RepeatStr(*str, times, seperator);
}
static std::string StringMultiply(std::string* str, uint32_t times)
{
    return RepeatStr(*str, times, "");
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
    // Finds an element in the array
    REGISTER_FUNCDEF("bool FindCallback(int index)", "Callback used for array find");
    REGISTER_METHOD_WRAPPER("array<T>", "T& find(FindCallback@ callback) const", ArrayFind);

    // *** String
    RegisterStdString(engine);
    RegisterStdStringUtils(engine);
    REGISTER_METHOD_WRAPPER("string", "string repeat(uint times, const string&in seperator = \"\") const", StringRepeat);
    REGISTER_METHOD_WRAPPER("string", "string opMul(uint times) const", StringMultiply);
    // Stringstream
    REGISTER_VALUE_CLASS("stringstream", std::stringstream, asOBJ_VALUE | asOBJ_POD, "A string stream");
    REGISTER_CONSTRUCTOR("stringstream", "", StringStreamDefaultConstructor);
    REGISTER_CONSTRUCTOR("stringstream", "const string&in str", StringStreamConstructor);
    REGISTER_METHOD_WRAPPER("stringstream", "string str() const", StringStreamToString);
    REGISTER_METHOD_WRAPPER("stringstream", "string opImplConv() const", StringStreamToString);
    REGISTER_METHOD_WRAPPER("stringstream", "string opConv() const", StringStreamToString);
    REGISTER_METHOD_WRAPPER("stringstream", "stringstream& opShl(const string&in)", StringStreamPipeIn);
    REGISTER_METHOD_WRAPPER("stringstream", "stringstream& opShl(?&in) const", StringStreamPipeInVarArg);
    REGISTER_METHOD_WRAPPER("stringstream", "stringstream& opShr(string&out) const", StringStreamPipeOut);
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
