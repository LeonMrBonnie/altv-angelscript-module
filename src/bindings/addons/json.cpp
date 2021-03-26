#include "../../helpers/module.h"
#include "../../helpers/angelscript.h"
#include "../../helpers/convert.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "nlohmann/json.hpp"

using namespace Helpers;
using namespace nlohmann;

static void JsonConstructor(void* memory)
{
    new(memory) json();
}
static void JsonConstructorDict(void* memory, CScriptDictionary* val)
{
    static int jsonTypeId = AngelScriptRuntime::Instance().GetEngine()->GetTypeIdByDecl("json::JSON");

    std::unordered_map<std::string, json> map;
    for(auto it : *val)
    {
        switch(it.GetTypeId())
        {
            case asTYPEID_BOOL: 
            {
                map.insert({ it.GetKey(), *(bool*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_INT8: 
            {
                map.insert({ it.GetKey(), *(int8_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_INT16: 
            {
                map.insert({ it.GetKey(), *(int16_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_INT32: 
            {
                map.insert({ it.GetKey(), *(int32_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_INT64: 
            {
                map.insert({ it.GetKey(), *(int64_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_UINT8: 
            {
                map.insert({ it.GetKey(), *(uint8_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_UINT16: 
            {
                map.insert({ it.GetKey(), *(uint16_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_UINT32: 
            {
                map.insert({ it.GetKey(), *(uint32_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_UINT64: 
            {
                map.insert({ it.GetKey(), *(uint64_t*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_FLOAT: 
            {
                map.insert({ it.GetKey(), *(float*)it.GetAddressOfValue() });
                break;
            }
            case asTYPEID_DOUBLE:
            {
                map.insert({ it.GetKey(), *(double*)it.GetAddressOfValue() });
                break;
            }
            default:
            {
                if(it.GetTypeId() == jsonTypeId) map.insert({ it.GetKey(), *(json*)it.GetAddressOfValue() });
                if(it.GetTypeId() == AngelScriptRuntime::Instance().GetStringTypeId()) map.insert({ it.GetKey(), *(std::string*)it.GetAddressOfValue() });
                break;
            }
        }
    }
    new(memory) json(map);
}
static void JsonConstructorArray(void* memory, CScriptArray* val)
{
    static int jsonTypeId = AngelScriptRuntime::Instance().GetEngine()->GetTypeIdByDecl("json::JSON");

    std::vector<json> vector;
    for(asUINT i = 0; i < val->GetSize(); i++)
    {
        auto typeId = val->GetElementTypeId();
        switch(typeId)
        {
            case asTYPEID_BOOL: 
            {
                vector.push_back(*(bool*)val->At(i));
                break;
            }
            case asTYPEID_INT8: 
            {
                vector.push_back(*(int8_t*)val->At(i));
                break;
            }
            case asTYPEID_INT16: 
            {
                vector.push_back(*(int16_t*)val->At(i));
                break;
            }
            case asTYPEID_INT32: 
            {
                vector.push_back(*(int32_t*)val->At(i));
                break;
            }
            case asTYPEID_INT64: 
            {
                vector.push_back(*(int64_t*)val->At(i));
                break;
            }
            case asTYPEID_UINT8: 
            {
                vector.push_back(*(uint8_t*)val->At(i));
                break;
            }
            case asTYPEID_UINT16: 
            {
                vector.push_back(*(uint16_t*)val->At(i));
                break;
            }
            case asTYPEID_UINT32: 
            {
                vector.push_back(*(uint32_t*)val->At(i));
                break;
            }
            case asTYPEID_UINT64: 
            {
                vector.push_back(*(uint64_t*)val->At(i));
                break;
            }
            case asTYPEID_FLOAT: 
            {
                vector.push_back(*(float*)val->At(i));
                break;
            }
            case asTYPEID_DOUBLE:
            {
                vector.push_back(*(double*)val->At(i));
                break;
            }
            default:
            {
                if(typeId == jsonTypeId) vector.push_back(*(json*)val->At(i));
                if(typeId == AngelScriptRuntime::Instance().GetStringTypeId()) vector.push_back(*(std::string*)val->At(i));
                break;
            }
        }
    }
    new(memory) json(vector);
}
static void JsonConstructorStr(void* memory, const std::string& val)
{
    new(memory) json(val);
}
static void JsonConstructorInt(void* memory, int64_t val)
{
    new(memory) json(val);
}
static void JsonConstructorUInt(void* memory, uint64_t val)
{
    new(memory) json(val);
}
static void JsonConstructorDouble(void* memory, double val)
{
    new(memory) json(val);
}
static void JsonConstructorBool(void* memory, bool val)
{
    new(memory) json(val);
}
static void JsonDestructor(void* memory)
{
    ((json*)memory)->~basic_json();
}

static json Parse(const std::string& jsonString)
{
    json parsed = json::parse(jsonString, nullptr, false, true);
    if(parsed.type() == json::value_t::discarded)
    {
        THROW_ERROR("Invalid JSON string");
        return nullptr;
    }
    return parsed;
}

static ModuleExtension utilExtension("json", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_VALUE_CLASS("JSON", json, asOBJ_VALUE, "A JSON class for storing a JSON value");
    REGISTER_DESTRUCTOR("JSON", JsonDestructor);
    REGISTER_CONSTRUCTOR("JSON", "", JsonConstructor);
    REGISTER_CONSTRUCTOR("JSON", "const string&in value", JsonConstructorStr);
    REGISTER_CONSTRUCTOR("JSON", "int64 value", JsonConstructorInt);
    REGISTER_CONSTRUCTOR("JSON", "uint64 value", JsonConstructorUInt);
    REGISTER_CONSTRUCTOR("JSON", "double value", JsonConstructorDouble);
    REGISTER_CONSTRUCTOR("JSON", "bool value", JsonConstructorBool);
    REGISTER_CONSTRUCTOR("JSON", "dictionary@ value", JsonConstructorDict);
    REGISTER_CONSTRUCTOR("JSON", "array<int8>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<int16>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<int>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<int64>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<uint8>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<uint16>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<uint>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<uint64>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<bool>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<double>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<float>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<string>@ value", JsonConstructorArray);
    REGISTER_CONSTRUCTOR("JSON", "array<JSON>@ value", JsonConstructorArray);

    engine->RegisterObjectMethod("JSON", "JSON &opIndex(const string &in)", 
        asMETHODPR(json, operator[], (const std::string&), json::reference), asCALL_THISCALL);
	engine->RegisterObjectMethod("JSON", "const JSON &opIndex(const string &in) const", 
        asMETHODPR(json, operator[], (const std::string&) const, json::const_reference), asCALL_THISCALL);
    engine->RegisterObjectMethod("JSON", "JSON &opAssign(const JSON &in)", 
        asMETHODPR(json, operator=, (const json), json::reference), asCALL_THISCALL);

    REGISTER_METHOD("JSON", "string ToString(int indent = 0)", json, dump);
    REGISTER_METHOD("JSON", "string opImplConv() const", json, dump);

    REGISTER_GLOBAL_FUNC("JSON Parse(const string&in jsonStr)", Parse, "Parses a JSON string to a JSON value");
});
