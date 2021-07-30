#pragma once

#include <regex>
#include "angelscript/include/angelscript.h"
#include "Log.h"

class AngelScriptResource;

namespace LibraryImport
{
    bool LibraryImportPragmaHandler(const std::string& pragmaStr, AngelScriptResource* resource);

    void FreeLibrary(void* library);
}  // namespace LibraryImport
