#include "helpers/events.h"
#include "helpers/convert.h"

using namespace Helpers;

#ifdef CLIENT_MODULE
// todo: figure out why these are broken
/*
REGISTER_EVENT_HANDLER(alt::CEvent::Type::SYNCED_META_CHANGE,
                       SyncedMetaChange,
                       "void",
                       "Entity@ entity, const string&in key, const ?&in value, const ?&in oldValue",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CSyncedMetaDataChangeEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           context->SetArgObject(1, (void*)ev->GetKey().CStr());
                           auto val = MValueToValue(resource->GetRuntime(), ev->GetVal());
                           context->SetArgVarType(2, std::get<1>(val), std::get<0>(val));
                           auto oldVal = MValueToValue(resource->GetRuntime(), ev->GetOldVal());
                           context->SetArgVarType(3, std::get<1>(oldVal), std::get<0>(oldVal));
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::STREAM_SYNCED_META_CHANGE,
                       StreamSyncedMetaChange,
                       "void",
                       "Entity@ entity, const string&in key, const ?&in value, const ?&in oldValue",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CStreamSyncedMetaDataChangeEvent*>(event);

                           context->SetArgObject(0, ev->GetTarget().Get());
                           context->SetArgObject(1, (void*)ev->GetKey().CStr());
                           auto val = MValueToValue(resource->GetRuntime(), ev->GetVal());
                           context->SetArgVarType(2, std::get<1>(val), std::get<0>(val));
                           auto oldVal = MValueToValue(resource->GetRuntime(), ev->GetOldVal());
                           context->SetArgVarType(3, std::get<1>(oldVal), std::get<0>(oldVal));
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::GLOBAL_SYNCED_META_CHANGE,
                       GlobalSyncedMetaChange,
                       "void",
                       "const string&in key, const ?&in value, const ?&in oldValue",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CGlobalSyncedMetaDataChangeEvent*>(event);

                           context->SetArgObject(0, (void*)ev->GetKey().CStr());
                           auto val = MValueToValue(resource->GetRuntime(), ev->GetVal());
                           context->SetArgVarType(1, std::get<1>(val), std::get<0>(val));
                           auto oldVal = MValueToValue(resource->GetRuntime(), ev->GetOldVal());
                           context->SetArgVarType(2, std::get<1>(oldVal), std::get<0>(oldVal));
                           return context->Execute();
                       });

REGISTER_EVENT_HANDLER(alt::CEvent::Type::GLOBAL_META_CHANGE,
                       GlobalMetaChange,
                       "void",
                       "const string&in key, const ?&in value, const ?&in oldValue",
                       [](AngelScriptResource* resource, const alt::CEvent* event, asIScriptContext* context) {
                           auto ev = static_cast<const alt::CGlobalMetaDataChangeEvent*>(event);

                           context->SetArgObject(0, (void*)ev->GetKey().CStr());
                           auto val = MValueToValue(resource->GetRuntime(), ev->GetVal());
                           context->SetArgVarType(1, std::get<1>(val), std::get<0>(val));
                           auto oldVal = MValueToValue(resource->GetRuntime(), ev->GetOldVal());
                           context->SetArgVarType(2, std::get<1>(oldVal), std::get<0>(oldVal));
                           return context->Execute();
                       });
*/
#endif
