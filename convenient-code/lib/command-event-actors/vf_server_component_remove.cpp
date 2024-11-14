#include "vf_server_component_remove.h"
#include "vcmp_componentdata.h"
#include "ve_commandevent.h"

QEvent *VfServerComponentRemove::generateEvent(int entityId, QString componentName)
{
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(entityId);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_REMOVE);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setComponentName(componentName);
    return new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
}
