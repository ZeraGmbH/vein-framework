#include "vftestentitycomponentcreator.h"
#include "vf_server_component_add.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>

bool VfTestEntityComponentCreator::createEntityComponent(int entityId, QString componentName, QVariant initialValue)
{
    bool alreadyInserted = m_entityComponentsCreated.contains(entityId) && m_entityComponentsCreated[entityId].contains(componentName);
    m_entityComponentsCreated[entityId].insert(componentName);
    createEntity(entityId);
    createComponent(entityId, componentName, initialValue);
    return !alreadyInserted;
}

void VfTestEntityComponentCreator::createEntity(int entityId)
{
    VeinComponent::EntityData *entityData = new VeinComponent::EntityData();
    entityData->setEntityId(entityId);
    entityData->setEventOrigin(VeinComponent::EntityData::EventOrigin::EO_LOCAL);
    entityData->setEventTarget(VeinComponent::EntityData::EventTarget::ET_ALL);
    entityData->setCommand(VeinComponent::EntityData::Command::ECMD_ADD);
    emit sigSendEvent(new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, entityData));
}

void VfTestEntityComponentCreator::createComponent(int entityId, QString componentName, QVariant initialValue)
{
    emit sigSendEvent(VfServerComponentAdd::generateEvent(entityId, componentName, initialValue));
}

void VfTestEntityComponentCreator::processEvent(QEvent *)
{
}
