#include "vfsimplegetter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>
#include "vcmp_errordata.h"

using namespace VeinEvent;
using namespace VeinComponent;

std::unique_ptr<VfSimpleGetter> VfSimpleGetter::create(int entityId, QString componentName)
{
    return std::make_unique<VfSimpleGetter>(entityId, componentName);
}

VfSimpleGetter::VfSimpleGetter(int entityId, QString componentName) :
    VfCommandEventHandlerItem(entityId),
    m_componentName(componentName)
{
}

void VfSimpleGetter::startGetComponent()
{
    ComponentData *cData = new ComponentData();
    cData->setEntityId(m_entityId);
    cData->setCommand(ComponentData::Command::CCMD_FETCH);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(m_componentName);
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    emit m_eventSystem->sigSendEvent(cEvent);
}

void VfSimpleGetter::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    if(cmdEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(cmdEvent);
        Q_ASSERT(cEvent != nullptr);
        EventData *evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);
        ComponentData *cData = static_cast<ComponentData *>(evData);
        if(cData->eventCommand() == ComponentData::Command::CCMD_FETCH && cData->componentName() == m_componentName)
            emit sigGetFinish(cData->newValue().isValid(), cData->newValue());
    }
}
