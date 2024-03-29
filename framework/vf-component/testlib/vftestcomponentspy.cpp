#include "vftestcomponentspy.h"
#include <ve_commandevent.h>

using VeinComponent::ComponentData;

VfTestComponentSpy::VfTestComponentSpy(ComponentData::Command cmdToFilter) :
    m_cmdToFilter(cmdToFilter)
{
}

QList<VfTestComponentSpy::TComponentInfo> VfTestComponentSpy::getComponentList() const
{
    return m_componentList;
}

void VfTestComponentSpy::reset()
{
    m_componentList.clear();
}

void VfTestComponentSpy::processEvent(QEvent *event)
{
    if(event->type() == VeinEvent::CommandEvent::getQEventType()) {
        VeinEvent::CommandEvent *cmdEvent = static_cast<VeinEvent::CommandEvent *>(event);
        if(cmdEvent != nullptr) {
            VeinEvent::EventData *evData = cmdEvent->eventData();
            if(evData->type() == ComponentData::dataType()) {
                ComponentData *componentData = static_cast<ComponentData *>(evData);
                if(componentData->eventCommand() == m_cmdToFilter) {
                    int entityId = evData->entityId();
                    QString componentName = componentData->componentName();
                    TComponentInfo info;
                    info.entityId = entityId;
                    info.componentName = componentName;
                    info.newValue = componentData->newValue();
                    info.oldValue = componentData->oldValue();
                    m_componentList.append(info);
                }
            }
        }
    }
}
