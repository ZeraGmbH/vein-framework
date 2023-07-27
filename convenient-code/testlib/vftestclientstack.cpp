#include "vftestclientstack.h"
#include "vfsimpleentitysubscriber.h"

VfTestClientStack::VfTestClientStack()
{
    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
}

void VfTestClientStack::subscribeEntityId(int entityId, VfCommandEventHandlerSystem *cmdEventHandlerSystem)
{
    VfSimpleEntitySubscriberPtr entityToSubscribe = VfSimpleEntitySubscriber::create(entityId);
    cmdEventHandlerSystem->addItem(entityToSubscribe);
    entityToSubscribe->sendSubscription();
    cmdEventHandlerSystem->removeItem(entityToSubscribe);
}
