#include "../../helpers/module.h"

using namespace Helpers;

static uint32_t StringToHash(const std::string& str)
{
    return alt::ICore::Instance().Hash(str);
}

static DataExtension hashExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    // Common joaat hash
    REGISTER_TYPEDEF("Hash", "uint32");

    engine->RegisterObjectMethod("string", "uint32 opImplConv() const", asFunctionPtr(StringToHash), asCALL_CDECL_OBJFIRST);
});
