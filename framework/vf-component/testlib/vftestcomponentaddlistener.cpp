#include "vftestcomponentaddlistener.h"
#include "vcmp_componentdata.h"
#include "ve_commandevent.h"
#include "ve_eventdata.h"

using VeinComponent::ComponentData;

void VfTestComponentAddListener::addComponentFilter(int entityId, QString componentName)
{
    m_componentFilter[entityId].insert(componentName);
}

VfTestComponentAddListener::AddHash VfTestComponentAddListener::getComponentHash()
{
    return m_componentAddHash;
}

bool VfTestComponentAddListener::processEvent(QEvent *t_event)
{
    if(t_event->type() == VeinEvent::CommandEvent::eventType()) {
        VeinEvent::CommandEvent *cmdEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
        if(cmdEvent != nullptr) {
            VeinEvent::EventData *evData = cmdEvent->eventData();
            if(evData->type() == ComponentData::dataType()) {
                ComponentData *componentData = static_cast<ComponentData *>(evData);

                if(componentData->eventCommand() == ComponentData::Command::CCMD_ADD) {
                    int entityId = evData->entityId();
                    QString componentName = componentData->componentName();
                    if(matchesComponentFilter(entityId, componentName)) {
                        const QVariant &newValue = componentData->newValue();
                        m_componentAddHash[entityId][componentName] = newValue;
                    }
                }
            }
        }
    }
    return false; // why is processEvent returning bool - it is ignored anyway?
}

bool VfTestComponentAddListener::matchesComponentFilter(int entityId, QString componentName)
{
    return m_componentFilter.isEmpty() ||
            (m_componentFilter.contains(entityId) &&
            m_componentFilter[entityId].contains(componentName));
}
