#include "vfcommandeventhandler.h"
#include "ve_eventdata.h"

using namespace VeinEvent;

VfCommandEventHandler::VfCommandEventHandler(CommandEvent::EventSubtype eventSubtypeFilter) :
    m_eventSubtypeFilter(eventSubtypeFilter)
{
}

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
        processCommandEvent(cmdEvent);
    }
}

void VfCommandEventHandler::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    if(cmdEvent->eventSubtype() == m_eventSubtypeFilter) {
        VeinEvent::EventData *eventData = cmdEvent->eventData();
        Q_ASSERT(eventData != nullptr);
        int entityId = eventData->entityId();
        auto iter = m_items.constFind(entityId);
        if(iter != m_items.constEnd()) {
            for(auto item : iter.value())
                item->processCommandEvent(cmdEvent);
        }
    }
}
