#include "vfentitywithrpcsimplified.h"
#include "ve_eventdata.h"
#include "vf_server_entity_add.h"
#include "vf_server_entity_remove.h"
#include "vcmp_remoteproceduredata.h"
#include "vcmp_errordatasender.h"

VfTEntityWithRpcSimplified::VfTEntityWithRpcSimplified(int entityId) :
    m_entityId(entityId)
{}

VfTEntityWithRpcSimplified::~VfTEntityWithRpcSimplified()
{
    emit sigSendEvent(VfServerEntityRemove::generateEvent(m_entityId));
}

void VfTEntityWithRpcSimplified::initModule()
{
    emit sigSendEvent(VfServerEntityAdd::generateEvent(m_entityId));
}

void VfTEntityWithRpcSimplified::createRpc(QString rpcName, QMap<QString, QString> parameters)
{
    VfCppRpcSimplifiedPtr rpc = std::make_shared<VfCppRpcSimplified>(this, m_entityId, rpcName, parameters);
    m_rpcList[rpc->getSignature()] = rpc;
}

void VfTEntityWithRpcSimplified::processEvent(QEvent *event)
{
    if(event->type()==VeinEvent::CommandEvent::getQEventType()) {
        VeinEvent::CommandEvent *cmdEvent = static_cast<VeinEvent::CommandEvent *>(event);
        VeinEvent::EventData *evData = cmdEvent->eventData();
        if(evData->entityId() == m_entityId) {
            if(cmdEvent->eventData()->type() == VeinComponent::RemoteProcedureData::dataType())
                handleRpcs(cmdEvent);
        }
    }
}

void VfTEntityWithRpcSimplified::handleRpcs(VeinEvent::CommandEvent *cmdEvent)
{
    VeinComponent::RemoteProcedureData *rpcData = static_cast<VeinComponent::RemoteProcedureData *>(cmdEvent->eventData());
    if(rpcData->command() == VeinComponent::RemoteProcedureData::Command::RPCMD_CALL) {
        if(m_rpcList.contains(rpcData->procedureName())) {
            const QUuid callId = rpcData->invokationData().value(VeinComponent::RemoteProcedureData::s_callIdString).toUuid();
            Q_ASSERT(!callId.isNull());
            m_rpcList[rpcData->procedureName()]->callFunction(callId, cmdEvent->peerId(), rpcData->invokationData());
            cmdEvent->accept();
        }
        else
            ErrorDataSender::errorOut(QString("No RPC with entityId: %1 name: %2").arg(m_entityId).arg(rpcData->procedureName()),
                                      cmdEvent,
                                      this);
    }
}
