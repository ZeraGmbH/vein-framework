#include "ve_commandevent.h"
#include "ve_eventdata.h"

namespace VeinEvent
{

const int CommandEvent::m_registeredQEventType = QEvent::registerEventType();

CommandEvent::CommandEvent(EventSubtype subtype, EventData *data) :
    QEvent(static_cast<QEvent::Type>(getQEventType())),
    m_subtype(subtype),
    m_eventData(data)
{
    Q_ASSERT(data != nullptr);
    Q_ASSERT(data->entityId() >= 0);
    if(!data->isValid())
        qWarning() << VEIN_DEBUGNAME_EVENT << "Invalid event data" << data;
    this->setAccepted(false);
}

CommandEvent::~CommandEvent()
{
    delete m_eventData;
}

int CommandEvent::getQEventType()
{
    return m_registeredQEventType;
}

const QUuid &CommandEvent::peerId() const
{
    return m_peerId;
}

void CommandEvent::setPeerId(const QUuid &peerId)
{
    m_peerId = peerId;
}

CommandEvent::EventSubtype CommandEvent::eventSubtype() const
{
    return m_subtype;
}

void CommandEvent::setEventSubtype(CommandEvent::EventSubtype newType)
{
    m_subtype = newType;
}

EventData *CommandEvent::eventData() const
{
    return m_eventData;
}

}
