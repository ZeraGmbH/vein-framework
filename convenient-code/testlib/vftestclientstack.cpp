#include "vftestclientstack.h"
#include "vfatomiccliententitysubscriber.h"

VfTestClientStack::VfTestClientStack()
{
    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
}

void VfTestClientStack::subscribeEntityId(int entityId, VfCommandEventHandlerSystem *cmdEventHandlerSystem)
{
    VfAtomicClientEntitySubscriberPtr entityToSubscribe = VfAtomicClientEntitySubscriber::create(entityId);
    cmdEventHandlerSystem->addItem(entityToSubscribe);
    entityToSubscribe->sendSubscription();
    cmdEventHandlerSystem->removeItem(entityToSubscribe);
}
