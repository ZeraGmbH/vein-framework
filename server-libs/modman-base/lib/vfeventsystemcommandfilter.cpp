#include "vfeventsystemcommandfilter.h"

int VfEventSystemCommandFilter::m_instanceCount = 0;

VfEventSystemCommandFilter::VfEventSystemCommandFilter(VeinEvent::CommandEvent::EventSubtype subtypeToFilter) :
    m_subtypeToFilter(subtypeToFilter)
{
    m_instanceCount++;
}

VfEventSystemCommandFilter::~VfEventSystemCommandFilter()
{
    m_instanceCount--;
}

void VfEventSystemCommandFilter::processEvent(QEvent *event)
{
    if(event->type() == VeinEvent::CommandEvent::getQEventType()) {
        VeinEvent::CommandEvent *commandEvent = static_cast<VeinEvent::CommandEvent *>(event);
        if(commandEvent && commandEvent->eventSubtype() == m_subtypeToFilter) {
            processCommandEvent(commandEvent);
        }
    }
}

int VfEventSystemCommandFilter::getInstanceCount()
{
    return m_instanceCount;
}
