#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

static void Construct(void* memory, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    new(memory) alt::RGBA(r, g, b, a);
}

namespace Data
{
    static void RegisterRGBA(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("RGBA", alt::RGBA, asOBJ_VALUE | asOBJ_POD, "R(ed), G(reen), B(lue), A(lpha) color");
        REGISTER_CONSTRUCTOR("RGBA", "uint8 r, uint8 g, uint8 b, uint8 a", Construct);
        REGISTER_PROPERTY("RGBA", "uint8 r", alt::RGBA, r);
        REGISTER_PROPERTY("RGBA", "uint8 g", alt::RGBA, g);
        REGISTER_PROPERTY("RGBA", "uint8 b", alt::RGBA, b);
        REGISTER_PROPERTY("RGBA", "uint8 a", alt::RGBA, a);
        REGISTER_METHOD("RGBA", "RGBA toBGRA()", alt::RGBA, toBGRA);
    }
}  // namespace Data
