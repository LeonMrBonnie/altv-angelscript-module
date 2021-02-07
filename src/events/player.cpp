#include "helpers/events.h"

using namespace Helpers;

REGISTER_EVENT_HANDLER(alt::CEvent::Type::PLAYER_CONNECT, PlayerConnect, "Player@ player",
[](AngelScriptResource* resource, const alt::CEvent* event, std::vector<std::pair<void*, bool>>& args) {
    auto ev = static_cast<const alt::CPlayerConnectEvent*>(event);
    args.push_back({ev->GetTarget().Get(), false});
});
