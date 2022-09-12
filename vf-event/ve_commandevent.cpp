#include "ve_commandevent.h"
#include "ve_eventdata.h"

namespace VeinEvent
{

CommandEvent::CommandEvent(EventSubtype t_subtype, EventData *t_data) :
    QEvent(static_cast<QEvent::Type>(eventType())),
    m_subtype(t_subtype),
    m_eventData(t_data)
{
    Q_ASSERT(t_data != nullptr);
    Q_ASSERT(t_data->entityId() >= 0);

    if(t_data->isValid() == false)
    {
        qCWarning(VEIN_EVENT) << "Invalid event data" << t_data;
    }
    this->setAccepted(false);
}

CommandEvent::~CommandEvent()
{
    delete m_eventData;
}


int CommandEvent::eventType()
{
    return s_eventType;
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

const int CommandEvent::s_eventType = QEvent::registerEventType();
}
