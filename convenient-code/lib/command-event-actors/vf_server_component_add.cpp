#include "vf_server_component_add.h"
#include "vcmp_componentdata.h"
#include "ve_commandevent.h"

QEvent *VfServerComponentAdd::generateEvent(int entityId, QString componentName, QVariant initialValue)
{
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(entityId);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_ADD);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setComponentName(componentName);
    cData->setNewValue(initialValue);
    return new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
}
