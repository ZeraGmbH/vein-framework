#include "vf_client_component_setter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

std::shared_ptr<VfClientComponentSetter> VfClientComponentSetter::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfClientComponentSetter>(componentName, entityItem);
}

VfClientComponentSetter::VfClientComponentSetter(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfComponentEventItem(componentName, entityItem)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

QEvent *VfClientComponentSetter::generateEvent(int entityId, QString componentName, QVariant oldValue, QVariant newValue)
{
    ComponentData *cData = new ComponentData(entityId, ComponentData::Command::CCMD_SET);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(componentName);

    // The following magic was stolen from vf-qml (now vf-qml re-uses this code)
    // Background: QML/JavaScript objects enter vein as QJSValue objects which we
    // do not support as meta objects.
    // Unfortunately we have to keep this workaround and cannot fix properly
    // due to:
    // * missing GUI tests
    // * ATTOW we found code in vf-logger checking for QVariantMap type when QJsonObject
    //   was sent - where else would code fail after a proper fix?
    // => Regressions caused are unpredictable :(
    if(Q_UNLIKELY(newValue.canConvert(QMetaType::QVariantList) && newValue.toList().isEmpty() == false))
        cData->setNewValue(newValue.toList());
    else if(Q_UNLIKELY(newValue.canConvert(QMetaType::QVariantMap)))
        cData->setNewValue(newValue.toMap());
    else
        cData->setNewValue(newValue);
    cData->setOldValue(oldValue);

    // connected client requires TRANSACTION - that is what servers listen to
    return new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
}


void VfClientComponentSetter::startSetComponent(QVariant oldValue, QVariant newValue)
{
    if(!newValue.isValid())
        emitSigSetFinish(false);
    else if(oldValue == newValue)
        emitSigSetFinish(true);
    else {
        QEvent *cEvent = generateEvent(getEntityId(), getComponentName(), oldValue, newValue);
        sendEvent(cEvent);
    }
}

void VfClientComponentSetter::processComponentEventData(const VeinComponent::ComponentData *componentData)
{
    if(componentData->eventCommand() == ComponentData::Command::CCMD_SET)
        emitSigSetFinish(true);
}

void VfClientComponentSetter::processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData)
{
    if(originalComponentData->eventCommand() == ComponentData::Command::CCMD_SET)
        emitSigSetFinish(false);
}

void VfClientComponentSetter::emitSigSetFinish(bool ok)
{
    QMetaObject::invokeMethod(this,
                              "sigSetFinish",
                              Qt::QueuedConnection,
                              Q_ARG(bool, ok));

}
