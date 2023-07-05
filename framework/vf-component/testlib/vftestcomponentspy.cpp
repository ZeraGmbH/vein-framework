#include "vftestcomponentspy.h"
#include <ve_commandevent.h>

using VeinComponent::ComponentData;

VfTestComponentSpy::VfTestComponentSpy(ComponentData::Command cmdToFilter) :
    m_cmdToFilter(cmdToFilter)
{
}

QList<VfTestComponentSpy::TComponentInfo> VfTestComponentSpy::getComponentList()
{
    return m_componentList;
}

bool VfTestComponentSpy::processEvent(QEvent *t_event)
{
    if(t_event->type() == VeinEvent::CommandEvent::eventType()) {
        VeinEvent::CommandEvent *cmdEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
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
    return false; // why is processEvent returning bool - it is ignored anyway?
}
