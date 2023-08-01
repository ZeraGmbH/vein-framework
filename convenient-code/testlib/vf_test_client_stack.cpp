#include "vf_test_client_stack.h"
#include "vf_client_entity_subscriber.h"

VfTestClientStack::VfTestClientStack()
{
    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
}

void VfTestClientStack::subscribeEntityId(int entityId, VfCmdEventHandlerSystem *cmdEventHandlerSystem)
{
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(entityId);
    cmdEventHandlerSystem->addItem(entityToSubscribe);
    entityToSubscribe->sendSubscription();
    cmdEventHandlerSystem->removeItem(entityToSubscribe);
}
