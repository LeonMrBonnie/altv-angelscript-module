#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/add_on/scriptarray/scriptarray.h"
#include <regex>

static void ConstructRegex(void* memory, const std::string& regex)
{
    new(memory) std::regex(regex);
}

static void DestructRegex(void* memory)
{
    ((std::regex*)memory)->~basic_regex();
}

static bool Test(std::regex* regex, const std::string& str)
{
    return std::regex_match(str, *regex);
}

static CScriptArray* Search(std::regex* regex, const std::string& str)
{
    std::smatch matches;
    bool        result = std::regex_search(str, matches, *regex);
    if(!result) return AngelScriptRuntime::Instance().CreateStringArray(0);

    CScriptArray* array = AngelScriptRuntime::Instance().CreateStringArray(matches.size());
    for(size_t i = 0; i < matches.size(); i++)
    {
        array->InsertAt(i, new std::string(matches[i].str()));
    }
    return array;
}

static CScriptArray* Match(std::regex* regex, const std::string& str)
{
    std::smatch matches;
    bool        result = std::regex_match(str, matches, *regex);
    if(!result) return AngelScriptRuntime::Instance().CreateStringArray(0);

    CScriptArray* array = AngelScriptRuntime::Instance().CreateStringArray(matches.size());
    for(size_t i = 0; i < matches.size(); i++)
    {
        array->InsertAt(i, new std::string(matches[i].str()));
    }
    return array;
}

static std::string* Replace(std::regex* regex, const std::string& str, const std::string& replaceVal, bool all)
{
    std::smatch matches;
    bool        result = std::regex_search(str, matches, *regex);
    if(!result) return new std::string();

    std::string* resultStr = new std::string(str);
    size_t       max       = all ? matches.size() : 1;
    for(size_t i = 0; i < max; i++)
    {
        resultStr->replace(matches[i].first, matches[i].second, replaceVal);
    }
    return resultStr;
}

static StdExtension regexExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_VALUE_CLASS("regex", std::regex, asOBJ_VALUE, "Regular expression");
    REGISTER_CONSTRUCTOR("regex", "const string&in regex", ConstructRegex);
    REGISTER_DESTRUCTOR("regex", DestructRegex);
    REGISTER_METHOD_WRAPPER("regex", "bool Test(const string&in str)", Test);
    REGISTER_METHOD_WRAPPER("regex", "array<string>@ Search(const string&in str)", Search);
    REGISTER_METHOD_WRAPPER("regex", "array<string>@ Match(const string&in str)", Match);
    REGISTER_METHOD_WRAPPER("regex", "string Replace(const string&in str, const string&in replaceVal, bool all = false)", Replace);
});
