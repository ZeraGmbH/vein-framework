#include "vf_server_component_setter.h"
#include "ve_commandevent.h"
#include <vcmp_componentdata.h>

using namespace VeinEvent;
using namespace VeinComponent;

QEvent *VfServerComponentSetter::generateEvent(int entityId, const QString &componentName, const QVariant &oldValue, const QVariant &newValue)
{
    ComponentData *cData = new ComponentData(entityId, ComponentData::Command::CCMD_SET);
    cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
    cData->setComponentName(componentName);

    cData->setNewValue(newValue);
    cData->setOldValue(oldValue);

    return new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, cData);
}
