#include "vfcommandeventhandlercomponentitem.h"
#include <vcmp_componentdata.h>

VfCommandEventHandlerComponentItem::VfCommandEventHandlerComponentItem(int entityId, QString componentName) :
    VfCommandEventHandlerItem(entityId),
    m_componentName(componentName)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfCommandEventHandlerComponentItem::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    Q_ASSERT(evData != nullptr);
    ComponentData *cData = static_cast<ComponentData *>(evData);
    Q_ASSERT(cData != nullptr);
    if(cData->componentName() == m_componentName)
        processComponentCommandEvent(cmdEvent);
}