#include "vf_server_rpc_invoker.h"
#include "ve_commandevent.h"
#include "vcmp_entitydata.h"
#include <vcmp_remoteproceduredata.h>

using namespace VeinEvent;
using namespace VeinComponent;

QEvent *VfServerRPCInvoker::generateEvent(int entityId, QString procedureName, QVariantMap parameters, QUuid identifier)
{
    QVariantMap rpcParamData;
    rpcParamData.insert(RemoteProcedureData::s_callIdString, identifier);
    rpcParamData.insert(RemoteProcedureData::s_parameterString, parameters);

    RemoteProcedureData *rpcData = new RemoteProcedureData();
    rpcData->setEntityId(entityId);
    rpcData->setCommand(RemoteProcedureData::Command::RPCMD_CALL);
    rpcData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    rpcData->setEventTarget(EntityData::EventTarget::ET_ALL);
    rpcData->setProcedureName(procedureName);
    rpcData->setInvokationData(rpcParamData);
    return new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, rpcData);
}
