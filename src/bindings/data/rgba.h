#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

static void Construct(void* memory, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    new(memory) alt::RGBA(r, g, b, a);
}
