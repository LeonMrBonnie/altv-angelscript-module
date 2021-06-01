#pragma once
#include "nlohmann/json.hpp"
#include "helpers/angelscript.h"

namespace JSON
{
    template<typename T>
    static void JsonConstructor(void* memory, T val)
    {
        new(memory) nlohmann::json(val);
    }
    static void JsonConstructorNull(void* memory)
    {
        new(memory) nlohmann::json();
    }
    static void JsonDestructor(void* memory)
    {
        ((nlohmann::json*)memory)->~basic_json();
    }

    static nlohmann::json Parse(const std::string& jsonString)
    {
        nlohmann::json parsed = nlohmann::json::parse(jsonString, nullptr, false, true);
        AS_ASSERT(parsed.type() != nlohmann::json::value_t::discarded, "Invalid JSON string", nlohmann::json());

        return parsed;
    }
}  // namespace JSON
