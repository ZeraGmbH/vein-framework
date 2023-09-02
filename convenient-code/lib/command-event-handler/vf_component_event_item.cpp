#include "vf_component_event_item.h"

VfComponentEventItem::VfComponentEventItem(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    m_componentName(componentName),
    m_entityItem(entityItem)
{
}

const QString &VfComponentEventItem::getComponentName() const
{
    return m_componentName;
}

VfCmdEventItemEntityPtr VfComponentEventItem::getEntityItem() const
{
    return m_entityItem;
}

VeinEvent::EventSystem *VfComponentEventItem::getEventSystem() const
{
    return m_entityItem->getEventSystem();
}
