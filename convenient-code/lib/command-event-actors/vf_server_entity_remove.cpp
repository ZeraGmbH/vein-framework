#include "vf_server_entity_remove.h"
#include "vcmp_entitydata.h"
#include "ve_commandevent.h"

QEvent *VfServerEntityRemove::generateEvent(int entityId)
{
    VeinComponent::EntityData *eData = new VeinComponent::EntityData();
    eData->setEntityId(entityId);
    eData->setCommand(VeinComponent::EntityData::Command::ECMD_REMOVE);
    return new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
}
