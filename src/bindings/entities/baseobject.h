#pragma once
#include "Log.h"
#include "../../helpers/module.h"
//#include "../../helpers/convert.h"

using namespace Helpers;

template<class T>
static void AddRef(T* obj)
{
    obj->AddRef();
}

template<class T>
static void RemoveRef(T* obj)
{
    obj->RemoveRef();
}

/*
template<class T>
static void GetMeta(const std::string& key, void* ref, int typeId, T* obj)
{
    GET_RESOURCE();
    if(!obj->HasMetaData(key))
    {
        THROW_ERROR("The specified meta key does not exist on the object");
        return;
    }

    // Dereference received handles to get the object
	if(typeId & asTYPEID_OBJHANDLE )
	{
		// Store the actual reference
		ref = *(void**)ref;
		typeId &= ~asTYPEID_OBJHANDLE;
	}
    asITypeInfo* type = resource->GetRuntime()->GetEngine()->GetTypeInfoById(typeId);
    auto mvalue = obj->GetMetaData(key);
    asITypeInfo* mvalueType = Helpers::GetTypeInfoFromMValue(resource->GetRuntime()->GetEngine(), mvalue);
    if(type != mvalueType)
    {
        THROW_ERROR("The specified output value for the meta data does not have the correct type");
        return;
    }
}
*/

namespace Helpers
{
    template<class T>
    static void RegisterAsBaseObject(asIScriptEngine* engine, DocsGenerator* docs, const char* type)
    {
        engine->RegisterObjectBehaviour(type, asBEHAVE_ADDREF, "void f()", asFUNCTION(AddRef<T>), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour(type, asBEHAVE_RELEASE, "void f()", asFUNCTION(RemoveRef<T>), asCALL_CDECL_OBJLAST);

        //REGISTER_PROPERTY_WRAPPER_GET(type, "uint8", "type", GetType<T>);
        REGISTER_PROPERTY_WRAPPER_GET(type, "uint8", "type", (GenericWrapper<T, alt::IBaseObject, &alt::IBaseObject::GetType, alt::IBaseObject::Type>));
        
        // todo: add meta methods
        //REGISTER_METHOD_WRAPPER(type, "void GetMeta(string key, ?&out outValue)", GetMeta<T>);
    }
}
