#include "vfcommandeventhandler.h"
#include "ve_eventdata.h"

using namespace VeinEvent;

VfCommandEventHandler::VfCommandEventHandler(CommandEvent::EventSubtype eventSubtypeFilter) :
    m_eventSubtypeFilter(eventSubtypeFilter)
{
}

void VfCommandEventHandler::addItem(VfCmdEventItemPtr item)
{
    ContainerSafeDeleteWhileLoop<VfCmdEventItemPtr> &items = m_items[item->getEntityId()];
    items.addElem(item);
}

void VfCommandEventHandler::removeItem(VfCmdEventItemPtr item)
{
    ContainerSafeDeleteWhileLoop<VfCmdEventItemPtr> &items = m_items[item->getEntityId()];
    items.removeElem(item);
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
        auto iter = m_items.find(entityId);
        if(iter != m_items.end()) {
            for(auto item = iter.value().getFirst(); item!=0; item=iter.value().getNext())
                item->processCommandEvent(cmdEvent);
        }
    }
}
