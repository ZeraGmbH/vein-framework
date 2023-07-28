#include "vfsimplegetter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

using namespace VeinEvent;
using namespace VeinComponent;

std::shared_ptr<VfSimpleGetter> VfSimpleGetter::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfSimpleGetter>(componentName, entityItem);
}

VfSimpleGetter::VfSimpleGetter(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent(componentName, entityItem)
{
}

void VfSimpleGetter::startGetComponent()
{
    ComponentData *cData = new ComponentData();
    cData->setEntityId(getEntityItem()->getEntityId());
    cData->setCommand(ComponentData::Command::CCMD_FETCH);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(getComponentName());
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    emit getEvenSystem()->sigSendEvent(cEvent);
}

void VfSimpleGetter::processComponentEventData(const ComponentData *cData)
{
    if(cData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(cData->newValue().isValid(), cData->newValue());
}
