#include "vfsimplegetter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

using namespace VeinEvent;
using namespace VeinComponent;

std::shared_ptr<VfSimpleGetter> VfSimpleGetter::create(int entityId, QString componentName)
{
    return std::make_shared<VfSimpleGetter>(entityId, componentName);
}

VfSimpleGetter::VfSimpleGetter(int entityId, QString componentName) :
    VfCommandEventHandlerComponentItem(entityId, componentName)
{
}

void VfSimpleGetter::startGetComponent()
{
    ComponentData *cData = new ComponentData();
    cData->setEntityId(getEntityId());
    cData->setCommand(ComponentData::Command::CCMD_FETCH);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(m_componentName);
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    emit m_eventSystem->sigSendEvent(cEvent);
}

void VfSimpleGetter::processComponentCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    Q_ASSERT(evData != nullptr);
    ComponentData *cData = static_cast<ComponentData *>(evData);
    Q_ASSERT(cData != nullptr);
    if(cData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(cData->newValue().isValid(), cData->newValue());
}
