#pragma once
#include "nlohmann/json.hpp"
#include "helpers/angelscript.h"
#include "angelscript/addon/scriptarray/scriptarray.h"

namespace JSON
{
    template<class T>
    static void JsonConstructorArray(void* memory, CScriptArray* val)
    {
        using namespace nlohmann;
        static int jsonTypeId = AngelScriptRuntime::Instance().GetEngine()->GetTypeIdByDecl("json::JSON");

        std::vector<json> vector;
        for(asUINT i = 0; i < val->GetSize(); i++)
        {
            vector.push_back(*(T*)val->At(i));
        }
        new(memory) json(vector);
    }
}  // namespace JSON
