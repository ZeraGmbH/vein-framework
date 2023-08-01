#include "vfcorestackclient.h"
#include "vfatomiccliententitysubscriber.h"

VfCoreStackClient::VfCoreStackClient()
{
    cmdEventHandlerSystem = VfCommandEventHandlerSystem::create();
    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    eventHandler.addSubsystem(cmdEventHandlerSystem.get());
}

void VfCoreStackClient::subscribeEntity(int entityId)
{
    VfAtomicClientEntitySubscriberPtr entityToSubscribe = VfAtomicClientEntitySubscriber::create(entityId);
    m_pendingCommandEventItems[entityToSubscribe.get()] = entityToSubscribe;
    cmdEventHandlerSystem->addItem(entityToSubscribe);
    connect(entityToSubscribe.get(), &VfAtomicClientEntitySubscriber::sigSubscribed,
            this, &VfCoreStackClient::onSubscribed);
    entityToSubscribe->sendSubscription();
}

void VfCoreStackClient::onSubscribed(bool ok)
{
    VfCmdEventItemPtr item = m_pendingCommandEventItems.take(sender());
    cmdEventHandlerSystem->removeItem(item);
    emit sigSubscribed(ok, item->getEntityId());
}
