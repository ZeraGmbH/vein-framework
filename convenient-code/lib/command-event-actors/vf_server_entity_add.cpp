#include "vf_server_entity_add.h"
#include "vcmp_entitydata.h"
#include "ve_commandevent.h"

QEvent *VfServerEntityAdd::generateEvent(int entityId)
{
    VeinComponent::EntityData *eData = new VeinComponent::EntityData();
    eData->setCommand(VeinComponent::EntityData::Command::ECMD_ADD);
    // Only subscribed are notified + subscription on non existant is not possible
    // Don't transfer over network
    eData->setEventTarget(VeinEvent::EventData::EventTarget::ET_LOCAL);
    eData->setEntityId(entityId);
    return new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
}
