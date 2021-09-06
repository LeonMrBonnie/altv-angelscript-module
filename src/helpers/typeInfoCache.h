#pragma once
#include "angelscript/include/angelscript.h"
#include "Log.h"
#include <string>
#include <unordered_map>

namespace Helpers
{
    class TypeInfoCache
    {
        std::unordered_map<std::string, asITypeInfo*> cache;

        TypeInfoCache(const TypeInfoCache&) = delete;
        TypeInfoCache(TypeInfoCache&&)      = delete;
        TypeInfoCache& operator=(const TypeInfoCache&) = delete;
        TypeInfoCache& operator=(TypeInfoCache&&) = delete;

    public:
        TypeInfoCache() {}

        void Add(asIScriptEngine* engine, const std::string& name)
        {
            auto typeInfo = engine->GetTypeInfoByDecl(name.c_str());
            if(!typeInfo)
            {
                Log::Error << "TypeInfoCache: Type info with name '" << name << "' not found. Please report this!" << Log::Endl;
                return;
            }
            cache.insert({ name, typeInfo });
#ifdef DEBUG_MODE
            Log::Info << "TypeInfoCache: Added cached type info for '" << name << "'" << Log::Endl;
#endif
        }

        asITypeInfo* Get(const std::string& name)
        {
            if(cache.count(name) == 0) return nullptr;
            return cache.at(name);
        }
    };
}  // namespace Helpers
