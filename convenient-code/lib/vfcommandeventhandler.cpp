#include "vfcommandeventhandler.h"
#include "ve_eventdata.h"

using namespace VeinEvent;

void VfCommandEventHandler::addItem(VfCommandEventHandlerItemPtr item)
{
    std::set<VfCommandEventHandlerItemPtr> &items = m_items[item->getEntityId()];
    items.insert(item);
}

void VfCommandEventHandler::removeItem(VfCommandEventHandlerItemPtr item)
{
    std::set<VfCommandEventHandlerItemPtr> &items = m_items[item->getEntityId()];
    items.erase(item);
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
    if(m_items.contains(entityId)) {
        for(auto item : m_items[entityId])
            item->processCommandEvent(cmdEvent);
    }
}
