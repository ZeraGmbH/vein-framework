#include "vfatomicclientcomponentsetter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

std::shared_ptr<VfAtomicClientComponentSetter> VfAtomicClientComponentSetter::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfAtomicClientComponentSetter>(componentName, entityItem);
}

VfAtomicClientComponentSetter::VfAtomicClientComponentSetter(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent(componentName, entityItem)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfAtomicClientComponentSetter::startSetComponent(QVariant oldValue, QVariant newValue)
{
    if(!newValue.isValid())
        emitSigSetFinish(false);
    else if(oldValue == newValue)
        emitSigSetFinish(true);
    else {
        ComponentData *cData = new ComponentData(getEntityItem()->getEntityId(), ComponentData::Command::CCMD_SET);
        cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
        cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
        cData->setComponentName(getComponentName());

        // The following magic was stopen from vf-qml
        if(Q_UNLIKELY(newValue.canConvert(QMetaType::QVariantList) && newValue.toList().isEmpty() == false))
            cData->setNewValue(newValue.toList());
        else if(Q_UNLIKELY(newValue.canConvert(QMetaType::QVariantMap)))
            cData->setNewValue(newValue.toMap());
        else
            cData->setNewValue(newValue);
        cData->setOldValue(oldValue);

        // connected client requires TRANSACTION - that is what servers listen to
        CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
        emit getEvenSystem()->sigSendEvent(cEvent);
    }
}

void VfAtomicClientComponentSetter::processComponentEventData(const VeinComponent::ComponentData *componentData)
{
    if(componentData->eventCommand() == ComponentData::Command::CCMD_SET)
        emitSigSetFinish(true);
}

void VfAtomicClientComponentSetter::processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData)
{
    if(originalComponentData->eventCommand() == ComponentData::Command::CCMD_SET)
        emitSigSetFinish(false);
}

void VfAtomicClientComponentSetter::emitSigSetFinish(bool ok)
{
    QMetaObject::invokeMethod(this,
                              "sigSetFinish",
                              Qt::QueuedConnection,
                              Q_ARG(bool, ok));

}
