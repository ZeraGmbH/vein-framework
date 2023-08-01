#include "vf_client_component_fetcher.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

std::shared_ptr<VfClientComponentFetcher> VfClientComponentFetcher::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfClientComponentFetcher>(componentName, entityItem);
}

VfClientComponentFetcher::VfClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfCmdEventItemComponent(componentName, entityItem)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfClientComponentFetcher::startGetComponent()
{
    ComponentData *cData = new ComponentData(getEntityItem()->getEntityId(), ComponentData::Command::CCMD_FETCH);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(getComponentName());
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
    emit getEvenSystem()->sigSendEvent(cEvent);
}

void VfClientComponentFetcher::processComponentEventData(const ComponentData *componentData)
{
    if(componentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(componentData->newValue().isValid(), componentData->newValue());
}

void VfClientComponentFetcher::processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData)
{
    if(originalComponentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(false, originalComponentData->newValue());

}
