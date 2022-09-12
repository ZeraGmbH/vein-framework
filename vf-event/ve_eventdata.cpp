#include "ve_eventdata.h"

namespace VeinEvent
{
EventData::EventData() :
    m_eventOrigin(EventOrigin::EO_LOCAL),
    m_eventTarget(EventTarget::ET_IRRELEVANT)
{
}

EventData::EventOrigin EventData::eventOrigin() const
{
    return m_eventOrigin;
}

void EventData::setEventOrigin(EventOrigin t_eventOrigin)
{
    m_eventOrigin = t_eventOrigin;
}

EventData::EventTarget EventData::eventTarget() const
{
    return m_eventTarget;
}

void EventData::setEventTarget(EventData::EventTarget t_eventTarget)
{
    m_eventTarget = t_eventTarget;
}

bool EventData::isValid() const
{
    return false;
}

int EventData::entityId() const
{
    return m_entityId;
}

void EventData::setEntityId(int t_entityId)
{
    if(t_entityId >= 0)
    {
        m_entityId = t_entityId;
    }
}
} // namespace VeinEvent
