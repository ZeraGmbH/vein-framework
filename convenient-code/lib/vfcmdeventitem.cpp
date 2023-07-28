#include "vfcmdeventitem.h"

VfCmdEventItem::VfCmdEventItem(int entityId) :
    m_entityId(entityId)
{
}

void VfCmdEventItem::setEventSystem(VeinEvent::EventSystem *eventSystem)
{
    m_eventSystem = eventSystem;
}

int VfCmdEventItem::getEntityId() const
{
    return m_entityId;
}
