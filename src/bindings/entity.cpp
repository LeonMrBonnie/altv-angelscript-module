#include "entity.h"

template<class T>
static T* Cast(alt::IEntity* entity)
{
    return dynamic_cast<T*>(entity);
}

static ModuleExtension entityExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsEntity<alt::IEntity>(engine, docs, "Entity");
    
    // Casts
    REGISTER_METHOD_WRAPPER("Entity", "alt::Player@ opCast() const", Cast<alt::IPlayer>);
    REGISTER_METHOD_WRAPPER("Entity", "alt::Vehicle@ opCast() const", Cast<alt::IVehicle>);
});
