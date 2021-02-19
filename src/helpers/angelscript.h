#pragma once

#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "./module.h"

namespace Helpers
{
    static void CopyAngelscriptValue(asIScriptEngine* engine, void* src, int srcType, void* dest, int destType)
    {
        if(destType & asTYPEID_OBJHANDLE && srcType & asTYPEID_MASK_OBJECT)
        {
            // RefCastObject will increment the refCount of the returned pointer if successful
            engine->RefCastObject(src, engine->GetTypeInfoById(srcType), engine->GetTypeInfoById(destType), reinterpret_cast<void**>(dest));
        }
        else if(destType & asTYPEID_MASK_OBJECT && srcType == destType)
        {
            engine->AssignScriptObject(dest, src, engine->GetTypeInfoById(srcType));
        }
        else
        {
            if(destType != srcType)
            {
                THROW_ERROR("The specified output value for the stream synced meta data does not have the correct type");
                return;
            }
            int size = engine->GetSizeOfPrimitiveType(destType);
            memcpy(dest, src, size);
        }
    }
}
