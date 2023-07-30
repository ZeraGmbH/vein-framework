#include "vfatomicclientcomponentfetcher.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

std::shared_ptr<VfAtomicClientComponentFetcher> VfAtomicClientComponentFetcher::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfAtomicClientComponentFetcher>(componentName, entityItem);
}

VfAtomicClientComponentFetcher::VfAtomicClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent(componentName, entityItem)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfAtomicClientComponentFetcher::startGetComponent()
{
    ComponentData *cData = new ComponentData(getEntityItem()->getEntityId(), ComponentData::Command::CCMD_FETCH);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(getComponentName());
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    emit getEvenSystem()->sigSendEvent(cEvent);
}

void VfAtomicClientComponentFetcher::processComponentEventData(const ComponentData *componentData)
{
    if(componentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(componentData->newValue().isValid(), componentData->newValue());
}

void VfAtomicClientComponentFetcher::processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData)
{
    if(originalComponentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(false, originalComponentData->newValue());

}
