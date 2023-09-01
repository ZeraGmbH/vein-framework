#include "vf_cmd_event_item.h"

VfCmdEventItem::VfCmdEventItem(int entityId) :
    m_entityId(entityId)
{
}

VeinEvent::EventSystem *VfCmdEventItem::getEventSystem() const
{
    return m_eventSystem;
}

void VfCmdEventItem::setEventSystem(VeinEvent::EventSystem *eventSystem)
{
    m_eventSystem = eventSystem;
}

int VfCmdEventItem::getEntityId() const
{
    return m_entityId;
}
