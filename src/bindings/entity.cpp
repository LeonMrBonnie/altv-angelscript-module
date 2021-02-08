#include "entity.h"

template<class T>
static T* Cast(alt::IEntity* entity)
{
    return dynamic_cast<T*>(entity);
}

static ModuleExtension entityExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_METHOD_WRAPPER("Entity", "alt::Player@ opCast() const", Cast<alt::IPlayer>);
    RegisterAsEntity<alt::IEntity>(engine, docs, "Entity");
});
