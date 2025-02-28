#include "vf_client_rpc_invoker.h"
#include "vcmp_entitydata.h"
#include <vcmp_remoteproceduredata.h>


std::shared_ptr<VfClientRPCInvoker> VfClientRPCInvoker::create(int entityId)
{
    return std::make_shared<VfClientRPCInvoker>(entityId);
}

VfClientRPCInvoker::VfClientRPCInvoker(int entityId) :
    VfEntityEventItem(entityId)
{
}
using namespace VeinEvent;
using namespace VeinComponent;

QEvent *VfClientRPCInvoker::generateEvent(int entityId, QString procedureName, QVariantMap parameters)
{
    QUuid rpcIdentifier = QUuid::createUuid();
    QVariantMap rpcParamData;
    rpcParamData.insert(RemoteProcedureData::s_callIdString, rpcIdentifier);
    rpcParamData.insert(RemoteProcedureData::s_parameterString, parameters);

    RemoteProcedureData *rpcData = new RemoteProcedureData();
    rpcData->setEntityId(entityId);
    rpcData->setCommand(RemoteProcedureData::Command::RPCMD_CALL);
    rpcData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    rpcData->setEventTarget(EntityData::EventTarget::ET_ALL);
    rpcData->setProcedureName(procedureName);
    rpcData->setInvokationData(rpcParamData);
    return new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, rpcData);
}

void VfClientRPCInvoker::invokeRPC(QString procedureName, QVariantMap paramters)
{
    QEvent *newEvent = generateEvent(getEntityId(), procedureName, paramters);
    sendEvent(newEvent);
}

void VfClientRPCInvoker::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    if(evData->type() == RemoteProcedureData::dataType()) {
        RemoteProcedureData *rpcData = static_cast<RemoteProcedureData *>(evData);
        if(rpcData->command() == RemoteProcedureData::Command::RPCMD_RESULT) {
            const QUuid rpcIdentifier = rpcData->invokationData().value(RemoteProcedureData::s_callIdString).toUuid(); //we don't need identifier
            emit sigRPCFinished(true, rpcIdentifier, rpcData->invokationData());
        }
    }
}

void VfClientRPCInvoker::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    if(originalEventData->type() == RemoteProcedureData::dataType()) {
        RemoteProcedureData *rpcData = static_cast<RemoteProcedureData *>(originalEventData);
        const QUuid rpcIdentifier = rpcData->invokationData().value(RemoteProcedureData::s_callIdString).toUuid();
        emit sigRPCFinished(false, rpcIdentifier, rpcData->invokationData());
    }
}


