#include "vf_client_rpc_invoker.h"
#include "vf_rpc_params_parser.h"
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

QEvent *VfClientRPCInvoker::generateEvent(int entityId, QString procedureName, QVariantMap parameters, QUuid identifier)
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
    return new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, rpcData);
}

QUuid VfClientRPCInvoker::invokeRPC(QString procedureName, QVariantMap paramters)
{
    QUuid identifier = QUuid::createUuid();
    m_pendingRPCs.insert(identifier);
    QString rpcSignature = VfRpcParamsParser::parseRpcParams(procedureName, paramters);
    QEvent *newEvent = generateEvent(getEntityId(), rpcSignature, paramters, identifier);
    sendEvent(newEvent);
    return identifier;
}

void VfClientRPCInvoker::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    if(evData->type() == RemoteProcedureData::dataType()) {
        RemoteProcedureData *rpcData = static_cast<RemoteProcedureData *>(evData);
        if(rpcData->command() == RemoteProcedureData::Command::RPCMD_RESULT) {
            const QUuid rpcIdentifier = rpcData->invokationData().value(RemoteProcedureData::s_callIdString).toUuid();
            if(m_pendingRPCs.contains(rpcIdentifier)) {
                m_pendingRPCs.remove(rpcIdentifier);
                emit sigRPCFinished(true, rpcIdentifier, rpcData->invokationData());
            }
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


