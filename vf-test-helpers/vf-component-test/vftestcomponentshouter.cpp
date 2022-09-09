#include "vftestcomponentshouter.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>

void VfTestComponentShouter::setValue(int entityId, QString componentName, QVariant newValue)
{
    if(m_value != newValue) {
        m_value = newValue;
        notifyValueChange(entityId, componentName);
    }
}

const QVariant &VfTestComponentShouter::getValue() const
{
    return m_value;
}

bool VfTestComponentShouter::processEvent(QEvent *)
{
    return false;
}

void VfTestComponentShouter::notifyValueChange(int entityId, QString componentName)
{
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(entityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    cData->setNewValue(m_value);

    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    event->setPeerId(QUuid());
    emit sigSendEvent(event);
}
