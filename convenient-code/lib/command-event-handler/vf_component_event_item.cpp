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

void VfComponentEventItem::sendEvent(QEvent *event) const
{
    m_entityItem->sendEvent(event);
}

int VfComponentEventItem::getEntityId() const
{
    return m_entityItem->getEntityId();
}
