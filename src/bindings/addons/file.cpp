#include "../../helpers/module.h"
#include "../../helpers/angelscript.h"

using namespace Helpers;

static std::string ReadFile(const std::string& path)
{
    GET_RESOURCE();
    auto file = resource->ReadFile(path);
    AS_ASSERT(!file.IsEmpty(), "File not found", std::string());

    return file.ToString();
}

static bool FileExists(const std::string& path)
{
    GET_RESOURCE();
    auto file = resource->ReadFile(path);
    return !file.IsEmpty();
}

#ifdef SERVER_MODULE
static std::string GetRootDir()
{
    return alt::ICore::Instance().GetRootDirectory().ToString();
}
#endif

static ModuleExtension fileExtension("file", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_GLOBAL_FUNC("string ReadFile(const string&in path)", ReadFile, "Reads the specified file contents");
    REGISTER_GLOBAL_FUNC("bool FileExists(const string&in path)", FileExists, "Checks if the given file exists");
#ifdef SERVER_MODULE
    REGISTER_GLOBAL_PROPERTY_READ_ONLY("string", "rootDir", GetRootDir);
#endif
});
