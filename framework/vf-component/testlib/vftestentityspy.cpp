#include "vftestentityspy.h"
#include <ve_commandevent.h>

using VeinComponent::EntityData;

VfTestEntitySpy::VfTestEntitySpy(EntityData::Command cmdToFilter) :
    m_cmdToFilter(cmdToFilter)
{
}

QList<int> VfTestEntitySpy::getEntityList() const
{
    return m_entityList;
}

void VfTestEntitySpy::reset()
{
    m_entityList.clear();
}

void VfTestEntitySpy::processEvent(QEvent *event)
{
    if(event->type() == VeinEvent::CommandEvent::eventType()) {
        VeinEvent::CommandEvent *cmdEvent = static_cast<VeinEvent::CommandEvent *>(event);
        if(cmdEvent != nullptr) {
            VeinEvent::EventData *evData = cmdEvent->eventData();
            if(evData->type() == EntityData::dataType()) {
                EntityData *entityData = static_cast<EntityData *>(evData);
                if(entityData->eventCommand() == m_cmdToFilter) {
                    int entityId = evData->entityId();
                    m_entityList.append(entityId);
                }
            }
        }
    }
}
