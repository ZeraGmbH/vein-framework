#include "vf_core_stack_client.h"
#include "vf_client_entity_subscriber.h"

VfCoreStackClient::VfCoreStackClient()
{
    cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    eventHandler.addSubsystem(cmdEventHandlerSystem.get());
}

void VfCoreStackClient::connectToServer(const QString &host, quint16 port)
{
    tcpSystem.connectToServer(host, port);
}

VeinNet::TcpSystem *VfCoreStackClient::getTcpSystem()
{
    return &tcpSystem;
}

void VfCoreStackClient::subscribeEntity(int entityId)
{
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(entityId);
    m_pendingCommandEventItems[entityToSubscribe.get()] = entityToSubscribe;
    cmdEventHandlerSystem->addItem(entityToSubscribe);
    connect(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed,
            this, &VfCoreStackClient::onSubscribed);
    entityToSubscribe->sendSubscription();
}

void VfCoreStackClient::onSubscribed(bool ok)
{
    VfCmdEventItemPtr item = m_pendingCommandEventItems.take(sender());
    cmdEventHandlerSystem->removeItem(item);
    emit sigSubscribed(ok, item->getEntityId());
}
