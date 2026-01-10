#include "vf_rpc_invoker.h"
#include "vf_rpc_params_parser.h"
#include <vcmp_remoteproceduredata.h>

std::shared_ptr<VfRPCInvoker> VfRPCInvoker::create(int entityId, std::unique_ptr<VfRPCEventGenerator> invokerType)
{
    return std::make_shared<VfRPCInvoker>(entityId, std::move(invokerType));
}

VfRPCInvoker::VfRPCInvoker(int entityId, std::unique_ptr<VfRPCEventGenerator> invokerType)
    : VfEntityEventItem(entityId), m_invokerType(std::move(invokerType))
{}

using namespace VeinEvent;
using namespace VeinComponent;

QUuid VfRPCInvoker::invokeRPC(QString procedureName, QVariantMap paramters)
{
    QUuid identifier = QUuid::createUuid();
    m_pendingRPCs.insert(identifier);
    QString rpcSignature = VfRpcParamsParser::parseRpcParams(procedureName, paramters);
    QEvent *newEvent = m_invokerType->generateEvent(getEntityId(), rpcSignature, paramters, identifier);
    sendEvent(newEvent);
    return identifier;
}

void VfRPCInvoker::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
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

void VfRPCInvoker::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    if(originalEventData->type() == RemoteProcedureData::dataType()) {
        RemoteProcedureData *rpcData = static_cast<RemoteProcedureData *>(originalEventData);
        const QUuid rpcIdentifier = rpcData->invokationData().value(RemoteProcedureData::s_callIdString).toUuid();
        if(m_pendingRPCs.contains(rpcIdentifier)) {
            m_pendingRPCs.remove(rpcIdentifier);
            emit sigRPCFinished(false, rpcIdentifier, rpcData->invokationData());
        }
    }
}
