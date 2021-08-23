#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scriptstdstring/scriptstdstring.h"

using namespace Helpers;

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

// *** Stringstream
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

static bool         registered = false;
extern StdExtension stringExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    if(registered) return;
    RegisterStdString(engine);

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

    registered = true;
});
