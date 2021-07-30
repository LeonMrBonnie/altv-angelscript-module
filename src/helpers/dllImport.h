#pragma once

#include <regex>
#include "angelscript/include/angelscript.h"
#include "Log.h"

class AngelScriptResource;

namespace DllImport
{
    bool DllImportPragmaHandler(const std::string& pragmaStr, AngelScriptResource* resource);

    void FreeDll(void* dll);
}  // namespace DllImport
