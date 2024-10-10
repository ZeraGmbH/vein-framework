#include "vftestentitycomponentcreator.h"
#include "vf_server_component_add.h"
#include "vf_server_entity_add.h"

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
    emit sigSendEvent(VfServerEntityAdd::generateEvent(entityId));
}

void VfTestEntityComponentCreator::createComponent(int entityId, QString componentName, QVariant initialValue)
{
    emit sigSendEvent(VfServerComponentAdd::generateEvent(entityId, componentName, initialValue));
}

void VfTestEntityComponentCreator::processEvent(QEvent *)
{
}
