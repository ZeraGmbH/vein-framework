#include "vf_server_rpc_register.h"
#include "vcmp_remoteproceduredata.h"
#include "ve_commandevent.h"

QEvent *VfServerRpcRegister::generateEvent(int entityId, QString rpcSignature)
{
    VeinComponent::RemoteProcedureData *rpcData = new VeinComponent::RemoteProcedureData();
    rpcData->setEntityId(entityId);
    rpcData->setCommand(VeinComponent::RemoteProcedureData::Command::RPCMD_REGISTER);
    rpcData->setProcedureName(rpcSignature);
    rpcData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    rpcData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    return new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, rpcData);
}
