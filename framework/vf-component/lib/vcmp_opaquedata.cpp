#include "vcmp_opaquedata.h"
#include <QBuffer>
#include <QDataStream>

namespace VeinComponent
{

OpaqueCommandEvent::OpaqueCommandEvent(std::unique_ptr<VeinEvent::CommandEvent> cmdEvent, int opaqueId) :
    m_cmdEvent(std::move(cmdEvent)),
    m_opaqueId(opaqueId)
{
    setEntityId(m_cmdEvent->eventData()->entityId());
}

int OpaqueCommandEvent::opaqueId() const
{
    return m_opaqueId;
}

std::unique_ptr<VeinEvent::CommandEvent> OpaqueCommandEvent::dragCommandEvent()
{
    Q_ASSERT(m_cmdEvent);
    return std::move(m_cmdEvent);
}

bool OpaqueCommandEvent::isValid() const
{
    return true;
}

QByteArray OpaqueCommandEvent::serialize() const
{
    QByteArray tmpData;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::WriteOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);
    dataStream << m_cmdEvent->eventData()->serialize();
    dataStream << m_opaqueId;

    dataBuffer.close();
    return tmpData;
}

void OpaqueCommandEvent::deserialize(const QByteArray &t_data)
{
    QByteArray tmpData=t_data;
    QBuffer dataBuffer(&tmpData);
    dataBuffer.open(QIODevice::ReadOnly);

    QDataStream dataStream(&dataBuffer);
    dataStream.setVersion(QDataStream::Qt_5_0);

    QByteArray tmpByteArray;
    dataStream >> tmpByteArray;
    m_cmdEvent->eventData()->deserialize(tmpByteArray);
    dataStream >> m_opaqueId;

    dataBuffer.close();
    setEntityId(m_cmdEvent->eventData()->entityId());
}

}
