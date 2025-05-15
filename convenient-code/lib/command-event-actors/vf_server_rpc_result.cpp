#include "vf_server_rpc_result.h"
#include "vcmp_remoteproceduredata.h"
#include "ve_commandevent.h"

VeinEvent::CommandEvent *VfServerRpcResult::generateEvent(int entityId, QString rpcSignature, QVariantMap returnValue)
{
    VeinComponent::RemoteProcedureData *resultData = new VeinComponent::RemoteProcedureData();
    resultData->setEntityId(entityId);
    resultData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    resultData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    resultData->setCommand(VeinComponent::RemoteProcedureData::Command::RPCMD_RESULT);
    resultData->setProcedureName(rpcSignature);
    resultData->setInvokationData(returnValue);
    return new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, resultData);
}
