#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/addon/scriptstdstring/scriptstdstring.h"

using namespace Helpers;

static inline void* CopyItem(asIScriptEngine* engine, void* item, int type)
{
    auto typeInfo = engine->GetTypeInfoById(type);

    // Item is an object handle
    if(type & asTYPEID_OBJHANDLE)
    {
        void* result = nullptr;
        engine->RefCastObject(item, typeInfo, typeInfo, &result);
        AS_ASSERT(result, "Failed to ref cast object", nullptr);
        return result;
    }
    // Item is a script object
    else if(type & asTYPEID_SCRIPTOBJECT)
    {
        return engine->CreateScriptObjectCopy(item, typeInfo);
    }
    // Item is a primitive
    else
    {
        auto  size = Helpers::GetTypeSize(type);
        void* copy = asAllocMem(size);
        memcpy(copy, item, size);
        return copy;
    }
}

static void ArrayPush(CScriptArray* array, void* item)
{
    array->InsertLast(item);
}
static void* ArrayPop(CScriptArray* array)
{
    void* item = array->At(array->GetSize() - 1);
    if(item == NULL) return NULL;
    array->RemoveLast();
    return item;
}
static void* ArrayShift(CScriptArray* array)
{
    void* item = array->At(0);
    if(item == NULL) return NULL;

    int  type     = array->GetElementTypeId();
    auto engine   = AngelScriptRuntime::Instance().GetEngine();
    auto typeInfo = engine->GetTypeInfoById(type);

    void* result = CopyItem(engine, item, type);
    array->RemoveAt(0);
    return result;
}
static void* ArrayFind(CScriptArray* array, asIScriptFunction* callback)
{
    asIScriptEngine*  engine  = AngelScriptRuntime::Instance().GetEngine();
    asUINT            size    = array->GetSize();
    int               type    = array->GetElementTypeId();
    asIScriptContext* context = asGetActiveContext();
    void*             result  = nullptr;

    for(asUINT i = 0; i < size; i++)
    {
        auto item = array->At(i);
        context->PushState();
        int r = context->Prepare(callback);
        CHECK_AS_RETURN("Find callback prepare", r, NULL);
        r = context->SetArgAddress(0, item);
        CHECK_AS_RETURN("Find callback set item", r, NULL);
        r = context->SetArgDWord(1, i);
        CHECK_AS_RETURN("Find callback set index", r, NULL);
        r = context->Execute();
        CHECK_AS_RETURN("Find callback execute", r, NULL);
        bool callResult = context->GetReturnByte();
        context->Unprepare();
        context->PopState();
        if(callResult)
        {
            result = CopyItem(engine, item, type);
            break;
        }
    }
    return result;
}

static StdExtension arrayExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterScriptArray(engine, true);

    // Alias for 'insertLast'
    REGISTER_METHOD_WRAPPER("array<T>", "void push(const T&in value)", ArrayPush);
    // Returns last item and removes it from array
    REGISTER_METHOD_WRAPPER("array<T>", "T& pop()", ArrayPop);
    // Returns first item and removes it from array
    REGISTER_METHOD_WRAPPER("array<T>", "T& shift()", ArrayShift);
    // Finds an element in the array
    REGISTER_FUNCDEF("bool array<T>::findCallback(const T&in if_handle_then_const item, const uint idx)", "Callback used for array find");
    REGISTER_METHOD_WRAPPER("array<T>", "T& find(const findCallback&in callback) const", ArrayFind);

    // We register this here because it needs the array template class
    RegisterStdStringUtils(engine);
});
