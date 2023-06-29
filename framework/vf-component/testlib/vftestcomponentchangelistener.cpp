#include "vftestcomponentchangelistener.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>

using VeinComponent::ComponentData;

void VfTestComponentChangeListener::addComponentToListen(int entityId, QString componentName)
{
    m_hashComponentValuesListening[entityId].insert(componentName);
}

QList<VfTestComponentChangeListener::TComponentInfo> VfTestComponentChangeListener::getComponentChangeList()
{
    return m_componentChangeList;
}

bool VfTestComponentChangeListener::processEvent(QEvent *t_event)
{
    if(t_event->type() == VeinEvent::CommandEvent::eventType()) {
        VeinEvent::CommandEvent *cmdEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
        if(cmdEvent != nullptr) {
            VeinEvent::EventData *evData = cmdEvent->eventData();
            if(evData->type() == ComponentData::dataType()) {
                ComponentData *componentData = static_cast<ComponentData *>(evData);
                if(componentData->eventCommand() == ComponentData::Command::CCMD_SET) {
                    int entityId = evData->entityId();
                    QString componentName = componentData->componentName();
                    if(wasComponentInserted(entityId, componentName)) {
                        const QVariant &oldValue = componentData->oldValue();
                        const QVariant &newValue = componentData->newValue();
                        if(oldValue != newValue) {
                            TComponentInfo info;
                            info.entityId = entityId;
                            info.componentName = componentName;
                            info.value = newValue;
                            m_componentChangeList.append(info);
                        }
                    }
                }
            }
        }
    }
    return false; // why is processEvent returning bool - it is ignored anyway?
}

bool VfTestComponentChangeListener::wasComponentInserted(int entityId, QString componentName)
{
    return m_hashComponentValuesListening.contains(entityId) &&
            m_hashComponentValuesListening[entityId].contains(componentName);
}
