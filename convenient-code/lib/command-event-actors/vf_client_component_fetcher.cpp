#include "vf_client_component_fetcher.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

std::shared_ptr<VfClientComponentFetcher> VfClientComponentFetcher::create(QString componentName, VfCmdEventItemEntityPtr entityItem)
{
    return std::make_shared<VfClientComponentFetcher>(componentName, entityItem);
}

VfClientComponentFetcher::VfClientComponentFetcher(QString componentName, VfCmdEventItemEntityPtr entityItem) :
    VfComponentEventItem(componentName, entityItem)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

QEvent *VfClientComponentFetcher::generateEvent(int entityId, QString componentName)
{
    ComponentData *cData = new ComponentData(entityId, ComponentData::Command::CCMD_FETCH);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(componentName);
    return new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, cData);
}

void VfClientComponentFetcher::startGetComponent()
{
    sendEvent(generateEvent(getEntityId(), getComponentName()));
}

void VfClientComponentFetcher::processComponentEventData(const ComponentData *componentData)
{
    if(componentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(true, componentData->newValue());
}

void VfClientComponentFetcher::processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData)
{
    if(originalComponentData->eventCommand() == ComponentData::Command::CCMD_FETCH)
        emit sigGetFinish(false, originalComponentData->newValue());

}
