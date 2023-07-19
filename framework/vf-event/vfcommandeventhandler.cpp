#include "vfcommandeventhandler.h"
#include "ve_eventdata.h"

using namespace VeinEvent;

void VfCommandEventHandler::addItem(VfCommandEventHandlerItemPtr item)
{
    int entityId = item->getEntityId();
    Q_ASSERT(!containsItem(entityId));
    m_items[entityId] = item;
}

void VfCommandEventHandler::removeItem(VfCommandEventHandlerItemPtr item)
{
    int entityId = item->getEntityId();
    Q_ASSERT(containsItem(entityId));
    m_items.erase(entityId);
}

bool VfCommandEventHandler::containsItem(int entityId)
{
    std::unordered_map<int, VfCommandEventHandlerItemPtr>::const_iterator iter = m_items.find(entityId);
    return iter != m_items.end();
}

void VfCommandEventHandler::processEvent(QEvent *event)
{
    if(event->type() == CommandEvent::eventType()) {
        CommandEvent *cmdEvent = static_cast<CommandEvent *>(event);
        Q_ASSERT(cmdEvent != nullptr);
        VeinEvent::EventData *eventData = cmdEvent->eventData();
        Q_ASSERT(eventData != nullptr);
        processCommandEvent(cmdEvent);
    }
}

void VfCommandEventHandler::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    VeinEvent::EventData *eventData = cmdEvent->eventData();
    Q_ASSERT(eventData != nullptr);
    int entityId = eventData->entityId();
    if(containsItem(entityId))
        m_items[entityId]->processCommandEvent(cmdEvent);
}
