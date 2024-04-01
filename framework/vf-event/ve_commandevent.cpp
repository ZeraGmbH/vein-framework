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

QUuid CommandEvent::peerId() const
{
    return m_peerId;
}

void CommandEvent::setPeerId(QUuid t_pPeerId)
{
    m_peerId = t_pPeerId;
}

CommandEvent::EventSubtype CommandEvent::eventSubtype() const
{
    return m_subtype;
}

void CommandEvent::setEventSubtype(CommandEvent::EventSubtype t_newType)
{
    m_subtype = t_newType;
}

EventData *CommandEvent::eventData() const
{
    return m_eventData;
}

}
