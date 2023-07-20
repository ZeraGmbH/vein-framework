#include "vfsimplepeer.h"
#include "ve_eventdata.h"


VfSimplePeer::VfSimplePeer()
{
}

int VfSimplePeer::m_transactionId = 0;

int VfSimplePeer::getNextTransactionId()
{
    return ++m_transactionId;
}

void VfSimplePeer::onSubscriptionFinish(int entityId)
{

}

using namespace VeinEvent;

void VfSimplePeer::processEvent(QEvent *event)
{
    if(event->type() == CommandEvent::eventType()) {
        CommandEvent *cmdEvent = static_cast<CommandEvent *>(event);
        Q_ASSERT(cmdEvent != nullptr);
        VeinEvent::EventData *eventData = cmdEvent->eventData();
        Q_ASSERT(eventData != nullptr);
        processCommandEvent(eventData->entityId(), cmdEvent);
    }
}

void VfSimplePeer::processCommandEvent(int entityId, CommandEvent *cmdEvent)
{

}
