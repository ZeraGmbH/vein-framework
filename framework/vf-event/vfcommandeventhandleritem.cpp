#include "vfcommandeventhandleritem.h"

VfCommandEventHandlerItem::VfCommandEventHandlerItem(int entityId) :
    m_entityId(entityId)
{
}

int VfCommandEventHandlerItem::getEntityId()
{
    return m_entityId;
}

void VfCommandEventHandlerItem::setEventSystem(VeinEvent::EventSystem *eventSystem)
{
    m_eventSystem = eventSystem;
}
