#include "vftestentity.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>

void VfTestEntity::createEntity(int entityId)
{
    VeinComponent::EntityData *entityData = new VeinComponent::EntityData();
    entityData->setCommand(VeinComponent::EntityData::Command::ECMD_ADD);
    entityData->setEventOrigin(VeinComponent::EntityData::EventOrigin::EO_LOCAL);
    entityData->setEventTarget(VeinComponent::EntityData::EventTarget::ET_ALL);
    entityData->setEntityId(entityId);
    emit sigSendEvent(new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, entityData));
}

bool VfTestEntity::processEvent(QEvent *)
{
    return false;
}
