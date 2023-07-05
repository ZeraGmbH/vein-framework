#include "vf-cpp-component.h"
#include <vcmp_componentdata.h>
#include <vcmp_errordata.h>
#include <ve_commandevent.h>
#include <ve_eventsystem.h>

using namespace VfCpp;

VfCppComponent::VfCppComponent(int entityId, VeinEvent::EventSystem *eventsystem, QString name, QVariant initval, bool readOnly) :
    m_nEntityId(entityId),
    m_pEventSystem(eventsystem),
    m_sName(name),
    m_vValue(initval),
    m_readOnly(readOnly)
{
    connect(m_pEventSystem, &QObject::destroyed, this, [&]{
        m_EventSystemDied = true;
    });
    sendNotification(VeinComponent::ComponentData::Command::CCMD_ADD);
}

VfCppComponent::~VfCppComponent()
{
    sendNotification(VeinComponent::ComponentData::Command::CCMD_REMOVE);
}

QVariant VfCppComponent::getValue()
{
    return m_vValue;
}

QString VfCppComponent::getName()
{
    return m_sName;
}

void VfCppComponent::setValue(QVariant value)
{
    m_vValue = value;
    sendNotification(VeinComponent::ComponentData::Command::CCMD_SET);
}

void VfCppComponent::setError()
{
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(m_nEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_SET);
    cData->setComponentName(m_sName);
    cData->setNewValue(m_vValue);

    VeinComponent::ErrorData *errData = new VeinComponent::ErrorData();
    errData->setEntityId(m_nEntityId);
    errData->setErrorDescription("Invalid parameter");
    errData->setOriginalData(cData);

    VeinEvent::CommandEvent *cEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, errData);
    QUuid id; // null id
    cEvent->setPeerId(id);

    emit m_pEventSystem->sigSendEvent(cEvent);
}

void VfCppComponent::setValueByEvent(QVariant value)
{
    if(value != getValue()) {
        if(!m_readOnly)
            setValue(value);
        emit sigValueChanged(value);
    }
}

void VfCppComponent::sendNotification(VeinComponent::ComponentData::Command vcmd)
{
    if(m_EventSystemDied)
        return;
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
