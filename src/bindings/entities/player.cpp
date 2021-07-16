#include "player.h"

using namespace Helpers;

static std::string ToString(alt::IPlayer* player)
{
    std::stringstream str;
    str << "Player{ id: " << std::to_string(player->GetID()) << ", name: " << player->GetName().ToString() << " }";
    return str.str();
}

#pragma region Client
#ifdef CLIENT_MODULE
static alt::IPlayer* GetLocalPlayer()
{
    return alt::ICore::Instance().GetLocalPlayer().Get();
}
#endif
#pragma endregion

static ModuleExtension playerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsPlayer<alt::IPlayer>(engine, docs, "Player");

    // Implicit conversion to string
    REGISTER_METHOD_WRAPPER("Player", "string opImplConv() const", ToString);
});
