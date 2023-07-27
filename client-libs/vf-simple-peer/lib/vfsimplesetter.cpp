#include "vfsimplesetter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>
#include "vcmp_errordata.h"

std::shared_ptr<VfSimpleSetter> VfSimpleSetter::create(int entityId, QString componentName)
{
    return std::make_shared<VfSimpleSetter>(entityId, componentName);
}

VfSimpleSetter::VfSimpleSetter(int entityId, QString componentName) :
    VfCommandEventHandlerItem(entityId),
    m_componentName(componentName)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfSimpleSetter::startSetComponent(QVariant oldValue, QVariant newValue)
{
    if(!newValue.isValid())
        emitSigSetFinish(false);
    else if(oldValue == newValue)
        emitSigSetFinish(true);
    else {
        ComponentData *cData = new ComponentData();
        cData->setEntityId(m_entityId);
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
        emit m_eventSystem->sigSendEvent(cEvent);
    }
}

void VfSimpleSetter::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    switch(evData->type()) {
    case ComponentData::dataType():
        doEmitSigSetFinish(true);
        break;
    case ErrorData::dataType():
        doEmitSigSetFinish(false);
        break;
    }
}

void VfSimpleSetter::emitSigSetFinish(bool ok)
{
    QMetaObject::invokeMethod(this,
                              "doEmitSigSetFinish",
                              Qt::QueuedConnection,
                              Q_ARG(bool, ok));

}

void VfSimpleSetter::doEmitSigSetFinish(bool ok)
{
    emit sigSetFinish(ok);
}
