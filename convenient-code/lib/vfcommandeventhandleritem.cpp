#include "vfcommandeventhandleritem.h"

VfCommandEventHandlerItem::VfCommandEventHandlerItem(int entityId) :
    m_entityId(entityId)
{
}

void VfCommandEventHandlerItem::setEventSystem(VeinEvent::EventSystem *eventSystem)
{
    m_eventSystem = eventSystem;
}

int VfCommandEventHandlerItem::getEntityId() const
{
    return m_entityId;
}
