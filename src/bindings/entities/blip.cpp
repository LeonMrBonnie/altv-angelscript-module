#include "Log.h"
#include "../../helpers/module.h"
#include "worldObject.h"

using namespace Helpers;
using namespace Data;

#ifdef SERVER_MODULE
static alt::IBlip* BlipFactory(alt::IPlayer* target, alt::IBlip::BlipType type, Vector3& pos)
{
    GET_RESOURCE();
    auto blip = alt::ICore::Instance().CreateBlip(target, type, { pos.x, pos.y, pos.z });
    AS_ASSERT(!blip.IsEmpty(), "Failed to create blip", nullptr);

    blip->AddRef();
    return blip.Get();
}

static alt::IBlip* BlipFactoryAttach(alt::IPlayer* target, alt::IBlip::BlipType type, alt::IEntity* attach)
{
    GET_RESOURCE();
    auto blip = alt::ICore::Instance().CreateBlip(target, type, attach);
    AS_ASSERT(!blip.IsEmpty(), "Failed to create blip", nullptr);

    blip->AddRef();
    return blip.Get();
}
#endif

static alt::IPlayer* GetTarget(alt::IBlip* blip)
{
    return blip->GetTarget().Get();
}

static alt::IEntity* GetAttachedTo(alt::IBlip* blip)
{
    return blip->AttachedTo().Get();
}

static ModuleExtension blipExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsWorldObject<alt::IBlip>(engine, docs, "Blip");

#ifdef SERVER_MODULE
    REGISTER_FACTORY("Blip", "Player@ target, uint16 type, Vector3 pos", BlipFactory);
    REGISTER_FACTORY("Blip", "Player@ target, uint16 type, Entity@ attachTo", BlipFactoryAttach);
#endif

    REGISTER_PROPERTY_WRAPPER_GET(
      "Blip", "uint16", "blipType", (GenericWrapper<alt::IBlip, alt::IBlip, &alt::IBlip::GetBlipType, alt::IBlip::BlipType>));

    REGISTER_PROPERTY_WRAPPER_GET("Blip", "bool", "global", (GenericWrapper<alt::IBlip, alt::IBlip, &alt::IBlip::IsGlobal, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Blip", "Player@+", "target", GetTarget);
    REGISTER_PROPERTY_WRAPPER_GET("Blip", "bool", "attached", (GenericWrapper<alt::IBlip, alt::IBlip, &alt::IBlip::IsAttached, bool>));
    REGISTER_PROPERTY_WRAPPER_GET("Blip", "Entity@+", "attachedTo", GetAttachedTo);
});
