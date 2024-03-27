#include "ve_commandevent.h"
#include "ve_eventdata.h"

namespace VeinEvent
{

const int CommandEvent::m_registeredQEventType = QEvent::registerEventType();

CommandEvent::CommandEvent(EventSubtype subtype, EventData *data) :
    QEvent(static_cast<QEvent::Type>(getQEventType())),
    m_subtype(subtype),
    m_eventData(data),
    m_activeEvent(this)
{
    Q_ASSERT(data != nullptr);
    Q_ASSERT(data->entityId() >= 0);
    if(data->isValid() == false)
        qWarning() << VEIN_DEBUGNAME_EVENT << "Invalid event data" << data;
    this->setAccepted(false);
}

CommandEvent::~CommandEvent()
{
    delete m_eventData;
}

void CommandEvent::mutateInto(std::unique_ptr<CommandEvent> otherCmdEvent)
{
    m_mutationCmdEvent = std::move(otherCmdEvent);
    m_activeEvent = m_mutationCmdEvent.get();
}

int CommandEvent::getQEventType()
{
    return m_registeredQEventType;
}

QUuid CommandEvent::peerId() const
{
    return m_activeEvent->m_peerId;
}

void CommandEvent::setPeerId(QUuid pPeerId)
{
    m_activeEvent->m_peerId = pPeerId;
}

CommandEvent::EventSubtype CommandEvent::eventSubtype() const
{
    return m_activeEvent->m_subtype;
}

void CommandEvent::setEventSubtype(CommandEvent::EventSubtype newType)
{
    m_activeEvent->m_subtype = newType;
}

EventData *CommandEvent::eventData() const
{
    return m_activeEvent->m_eventData;
}

}
