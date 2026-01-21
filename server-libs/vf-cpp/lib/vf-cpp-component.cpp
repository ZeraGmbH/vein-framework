#include "vf-cpp-component.h"
#include <vcmp_componentdata.h>
#include <ve_commandevent.h>
#include <ve_eventsystem.h>

using namespace VfCpp;

VfCppComponent::VfCppComponent(int entityId,
                               VeinEvent::EventSystem *eventsystem,
                               const QString &name,
                               const QVariant &initval,
                               bool readOnly) :
    m_nEntityId(entityId),
    m_pEventSystem(eventsystem),
    m_sName(name),
    m_vValue(initval),
    m_readOnly(readOnly)
{
    sendNotification(VeinComponent::ComponentData::Command::CCMD_ADD);
}

QVariant VfCppComponent::getValue()
{
    return m_vValue;
}

QString VfCppComponent::getName()
{
    return m_sName;
}

void VfCppComponent::setValue(const QVariant &value)
{
    m_vValue = value;
    sendNotification(VeinComponent::ComponentData::Command::CCMD_SET);
}

void VfCppComponent::setValueFromVein(const QVariant &value)
{
    if(value != getValue()) {
        if(!m_readOnly)
            setValue(value);
        emit sigValueChanged(value);
    }
}

void VfCppComponent::changeComponentReadWriteType(bool readWrite)
{
    m_readOnly = readWrite;
}

void VfCppComponent::sendNotification(VeinComponent::ComponentData::Command vcmd)
{
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(m_nEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(vcmd);
    cData->setComponentName(m_sName);
    cData->setNewValue(m_vValue);

    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    QUuid id; // null id
    event->setPeerId(id);
    emit m_pEventSystem->sigSendEvent(event);
}
