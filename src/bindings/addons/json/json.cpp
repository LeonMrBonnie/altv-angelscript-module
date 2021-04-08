#include "helpers/module.h"
#include "./array.h"
#include "./dict.h"
#include "./generic.h"

static ModuleExtension utilExtension("json", [](asIScriptEngine* engine, DocsGenerator* docs) {
    using namespace JSON;
    using namespace nlohmann;

    REGISTER_VALUE_CLASS("JSON", json, asOBJ_VALUE, "A JSON class for storing a JSON value");
    REGISTER_DESTRUCTOR("JSON", JsonDestructor);
    REGISTER_CONSTRUCTOR("JSON", "", JsonConstructorNull);
    REGISTER_CONSTRUCTOR("JSON", "const string&in value", JsonConstructor<const std::string&>);
    REGISTER_CONSTRUCTOR("JSON", "int64 value", JsonConstructor<int64_t>);
    REGISTER_CONSTRUCTOR("JSON", "uint64 value", JsonConstructor<uint64_t>);
    REGISTER_CONSTRUCTOR("JSON", "double value", JsonConstructor<double>);
    REGISTER_CONSTRUCTOR("JSON", "bool value", JsonConstructor<bool>);
    REGISTER_CONSTRUCTOR("JSON", "dictionary@ value", JsonConstructorDict);
    REGISTER_CONSTRUCTOR("JSON", "array<int8>@ value", JsonConstructorArray<int8_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<int16>@ value", JsonConstructorArray<int16_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<int>@ value", JsonConstructorArray<int32_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<int64>@ value", JsonConstructorArray<int64_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<uint8>@ value", JsonConstructorArray<uint8_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<uint16>@ value", JsonConstructorArray<uint16_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<uint>@ value", JsonConstructorArray<uint32_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<uint64>@ value", JsonConstructorArray<uint64_t>);
    REGISTER_CONSTRUCTOR("JSON", "array<bool>@ value", JsonConstructorArray<bool>);
    REGISTER_CONSTRUCTOR("JSON", "array<double>@ value", JsonConstructorArray<double>);
    REGISTER_CONSTRUCTOR("JSON", "array<float>@ value", JsonConstructorArray<float>);
    REGISTER_CONSTRUCTOR("JSON", "array<string>@ value", JsonConstructorArray<std::string>);
    REGISTER_CONSTRUCTOR("JSON", "array<JSON>@ value", JsonConstructorArray<json>);

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
