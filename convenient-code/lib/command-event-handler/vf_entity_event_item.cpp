#include "vf_entity_event_item.h"

VfEntityEventItem::VfEntityEventItem(int entityId) :
    m_entityId(entityId)
{
}

VeinEvent::EventSystem *VfEntityEventItem::getEventSystem() const
{
    return m_eventSystem;
}

void VfEntityEventItem::setEventSystem(VeinEvent::EventSystem *eventSystem)
{
    m_eventSystem = eventSystem;
}

int VfEntityEventItem::getEntityId() const
{
    return m_entityId;
}
