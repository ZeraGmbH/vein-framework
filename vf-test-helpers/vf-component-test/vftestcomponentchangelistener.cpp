#include "vftestcomponentchangelistener.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>

using VeinComponent::ComponentData;

VfTestComponentChangeListener::VfTestComponentChangeListener(VeinStorage::VeinHash *storageHash) :
    m_storageHash(storageHash)
{
}

void VfTestComponentChangeListener::addComponentToListen(QString componentName, const QVariant *componentValue)
{
    m_hashComponentValuesListening[componentName] = componentValue;
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
                ComponentData *cData = static_cast<ComponentData *>(evData);
                if(cData->eventCommand() == ComponentData::Command::CCMD_SET) {
                    QString componentName = cData->componentName();
                    auto iter = m_hashComponentValuesListening.find(componentName);
                    if(iter != m_hashComponentValuesListening.end()) {
                        int entityId = evData->entityId();
                        QVariant oldValue = m_storageHash->getStoredValue(entityId, componentName);
                        const QVariant newValue = *iter.value();
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
