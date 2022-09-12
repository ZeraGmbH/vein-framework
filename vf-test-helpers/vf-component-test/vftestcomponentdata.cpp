#include "vftestcomponentdata.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>

void VfTestComponentData::setValue(int entityId, QString componentName, QVariant newValue)
{
    QVariant &hashValue = m_valueHash[entityId][componentName];
    if(hashValue != newValue) {
        hashValue = newValue;
        notifyValueChange(entityId, componentName, newValue);
    }
}

QVariant VfTestComponentData::getValue(int entityId, QString componentName) const
{
    return m_valueHash[entityId][componentName];
}

bool VfTestComponentData::processEvent(QEvent *)
{
    return false;
}

void VfTestComponentData::notifyValueChange(int entityId, QString componentName, QVariant newValue)
{
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(entityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    cData->setNewValue(newValue);

    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    emit sigSendEvent(event);
}
