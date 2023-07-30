#include "vfatomicclientcomponentgetter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

using namespace VeinEvent;
using namespace VeinComponent;

std::shared_ptr<VfAtomicClientComponentGetter> VfAtomicClientComponentGetter::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfAtomicClientComponentGetter>(componentName, entityItem);
}

VfAtomicClientComponentGetter::VfAtomicClientComponentGetter(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent(componentName, entityItem)
{
}

void VfAtomicClientComponentGetter::startGetComponent()
{
    ComponentData *cData = new ComponentData(getEntityItem()->getEntityId(), ComponentData::Command::CCMD_FETCH);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(getComponentName());
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    emit getEvenSystem()->sigSendEvent(cEvent);
}

void VfAtomicClientComponentGetter::processComponentEventData(const ComponentData *componentData)
{
    if(componentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(componentData->newValue().isValid(), componentData->newValue());
}

void VfAtomicClientComponentGetter::processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData)
{
    if(originalComponentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(false, originalComponentData->newValue());

}
