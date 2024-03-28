#include "vf_core_stack_client.h"
#include "vf_client_entity_subscriber.h"

VfCoreStackClient::VfCoreStackClient()
{
    m_cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    m_netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    m_eventHandler.addSubsystem(&m_netSystem);
    m_eventHandler.addSubsystem(&m_tcpSystem);
    m_eventHandler.addSubsystem(m_cmdEventHandlerSystem.get());
}

void VfCoreStackClient::connectToServer(const QString &host, quint16 port)
{
    m_tcpSystem.connectToServer(host, port);
}

VeinNet::TcpSystem *VfCoreStackClient::getTcpSystem()
{
    return &m_tcpSystem;
}

void VfCoreStackClient::subscribeEntity(int entityId)
{
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(entityId);
    m_pendingCommandEventItems[entityToSubscribe.get()] = entityToSubscribe;
    m_cmdEventHandlerSystem->addItem(entityToSubscribe);
    connect(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed,
            this, &VfCoreStackClient::onSubscribed);
    entityToSubscribe->sendSubscription();
}

void VfCoreStackClient::onSubscribed(bool ok)
{
    VfCmdEventItemPtr item = m_pendingCommandEventItems.take(sender());
    m_cmdEventHandlerSystem->removeItem(item);
    emit sigSubscribed(ok, item->getEntityId());
}
