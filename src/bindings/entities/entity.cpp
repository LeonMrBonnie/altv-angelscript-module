#include "entity.h"

template<class T, alt::IBaseObject::Type type>
static T* Cast(alt::IEntity* entity)
{
    if(entity->GetType() != type)
    {
        THROW_ERROR("Entity has wrong type");
        return nullptr;
    }
    return dynamic_cast<T*>(entity);
}

static ModuleExtension entityExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsEntity<alt::IEntity>(engine, docs, "Entity");

    // Casts
    REGISTER_METHOD_WRAPPER("Entity", "alt::Player@ opCast() const", (Cast<alt::IPlayer, alt::IBaseObject::Type::PLAYER>));
    REGISTER_METHOD_WRAPPER("Entity", "alt::Vehicle@ opCast() const", (Cast<alt::IVehicle, alt::IBaseObject::Type::VEHICLE>));

    // todo: add config option to toggle this
    // !!! this can cause nasty bugs, use with caution
    REGISTER_METHOD_WRAPPER("Entity", "alt::Player@ opImplCast() const", (Cast<alt::IPlayer, alt::IBaseObject::Type::PLAYER>));
    REGISTER_METHOD_WRAPPER("Entity", "alt::Vehicle@ opImplCast() const", (Cast<alt::IVehicle, alt::IBaseObject::Type::VEHICLE>));
});
