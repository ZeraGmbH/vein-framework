#include "vf_entity_event_item.h"

VfEntityEventItem::VfEntityEventItem(int entityId) :
    m_entityId(entityId)
{
}

void VfEntityEventItem::sendEvent(QEvent *event) const
{
    emit m_eventSystem->sigSendEvent(event);
}

void VfEntityEventItem::setEventSystem(VeinEvent::EventSystem *eventSystem)
{
    m_eventSystem = eventSystem;
}

int VfEntityEventItem::getEntityId() const
{
    return m_entityId;
}
