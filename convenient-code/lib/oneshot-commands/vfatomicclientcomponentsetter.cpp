#include "vfatomicclientcomponentsetter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>
#include "vcmp_errordata.h"

std::shared_ptr<VfAtomicClientComponentSetter> VfAtomicClientComponentSetter::create(int entityId, QString componentName)
{
    return std::make_shared<VfAtomicClientComponentSetter>(entityId, componentName);
}

VfAtomicClientComponentSetter::VfAtomicClientComponentSetter(int entityId, QString componentName) :
    VfCmdEventItem(entityId),
    m_componentName(componentName)
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
        ComponentData *cData = new ComponentData();
        cData->setEntityId(getEntityId());
        cData->setCommand(ComponentData::Command::CCMD_SET);
        cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
        cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
        cData->setComponentName(m_componentName);
        if(Q_UNLIKELY(newValue.canConvert(QMetaType::QVariantList) && newValue.toList().isEmpty() == false))
            cData->setNewValue(newValue.toList());
        else if(Q_UNLIKELY(newValue.canConvert(QMetaType::QVariantMap)))
            cData->setNewValue(newValue.toMap());
        else
            cData->setNewValue(newValue);
        cData->setOldValue(oldValue);

        CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
        emit getEvenSystem()->sigSendEvent(cEvent);
    }
}

void VfAtomicClientComponentSetter::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    switch(evData->type()) {
    case ComponentData::dataType():
        emitSigSetFinish(true);
        break;
    case ErrorData::dataType():
        emitSigSetFinish(false);
        break;
    }
}

void VfAtomicClientComponentSetter::emitSigSetFinish(bool ok)
{
    QMetaObject::invokeMethod(this,
                              "sigSetFinish",
                              Qt::QueuedConnection,
                              Q_ARG(bool, ok));

}
