#include "vf_cmd_event_item_component.h"

VfCmdEventItemComponent::VfCmdEventItemComponent(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    m_componentName(componentName),
    m_entityItem(entityItem)
{
}

const QString &VfCmdEventItemComponent::getComponentName() const
{
    return m_componentName;
}

VfCmdEventItemEntityPtr VfCmdEventItemComponent::getEntityItem() const
{
    return m_entityItem;
}

VeinEvent::EventSystem *VfCmdEventItemComponent::getEventSystem() const
{
    return m_entityItem->getEventSystem();
}
