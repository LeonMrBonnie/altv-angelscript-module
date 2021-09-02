#include "Log.h"
#include "player.h"

using namespace Helpers;

#ifdef CLIENT_MODULE
static std::string ToString(alt::ILocalPlayer* player)
{
    std::stringstream str;
    str << "LocalPlayer{ id: " << std::to_string(player->GetID()) << ", name: " << player->GetName().ToString() << " }";
    return str.str();
}

static alt::ILocalPlayer* GetLocalPlayer()
{
    return alt::ICore::Instance().GetLocalPlayer().Get();
}

static ModuleExtension localPlayerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsPlayer<alt::ILocalPlayer>(engine, docs, "LocalPlayer");

    REGISTER_GLOBAL_PROPERTY_READ_ONLY("LocalPlayer@+", "localPlayer", GetLocalPlayer);

    REGISTER_PROPERTY_WRAPPER_GET(
      "LocalPlayer", "uint16", "currentAmmo", (GenericWrapper<alt::ILocalPlayer, alt::ILocalPlayer, &alt::ILocalPlayer::GetCurrentAmmo, uint16_t>));

    // Implicit conversion to string
    REGISTER_METHOD_WRAPPER("LocalPlayer", "string opImplConv() const", ToString);
});
#endif
