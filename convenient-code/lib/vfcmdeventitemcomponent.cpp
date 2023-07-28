#include "vfcmdeventitemcomponent.h"

VfCmdEventItemComponent::VfCmdEventItemComponent(int entityId, QString componentName) :
    VfCmdEventItem(entityId),
    m_componentName(componentName)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfCmdEventItemComponent::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    Q_ASSERT(evData != nullptr);
    if(evData->type() == ComponentData::dataType()) {
        ComponentData *cData = static_cast<ComponentData *>(evData);
        Q_ASSERT(cData != nullptr);
        if(cData->componentName() == m_componentName)
            processComponentEventData(cData);
    }
}
