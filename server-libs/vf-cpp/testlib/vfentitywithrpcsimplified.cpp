#include "vfentitywithrpcsimplified.h"
#include "rpcfortest.h"
#include "rpcadddelay.h"
#include "ve_eventdata.h"
#include "vf_server_entity_add.h"
#include "vf_server_entity_remove.h"
#include "vcmp_remoteproceduredata.h"
#include "vcmp_errordatasender.h"

VfEntityWithRpcSimplified::VfEntityWithRpcSimplified(int entityId) :
    m_entityId(entityId)
{}

VfEntityWithRpcSimplified::~VfEntityWithRpcSimplified()
{
    emit sigSendEvent(VfServerEntityRemove::generateEvent(m_entityId));
}

void VfEntityWithRpcSimplified::initModule()
{
    emit sigSendEvent(VfServerEntityAdd::generateEvent(m_entityId));
    m_rpcHandlerList.append(std::make_shared<RpcForTest>(this, m_entityId));
    m_rpcHandlerList.append(std::make_shared<RpcAddDelay>(this, m_entityId));
}

void VfEntityWithRpcSimplified::processEvent(QEvent *event)
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

void VfEntityWithRpcSimplified::handleRpcs(VeinEvent::CommandEvent *cmdEvent)
{
    VeinComponent::RemoteProcedureData *rpcData = static_cast<VeinComponent::RemoteProcedureData *>(cmdEvent->eventData());
    if(rpcData->command() == VeinComponent::RemoteProcedureData::Command::RPCMD_CALL) {
        const QUuid callId = rpcData->invokationData().value(VeinComponent::RemoteProcedureData::s_callIdString).toUuid();
        Q_ASSERT(!callId.isNull());
        bool rpcFound = false;
        for(auto rpc: m_rpcHandlerList) {
            if(rpc->getSignature() == rpcData->procedureName()) {
                rpcFound = true;
                QVariantMap params = rpcData->invokationData().value(VeinComponent::RemoteProcedureData::s_parameterString).toMap();
                rpc->callFunction(callId, cmdEvent->peerId(), params);
                cmdEvent->accept();
            }
        }
        if(!rpcFound)
            ErrorDataSender::errorOut(QString("No RPC with entityId: %1 name: %2").arg(m_entityId).arg(rpcData->procedureName()),
                                      cmdEvent,
                                      this);
    }
}
