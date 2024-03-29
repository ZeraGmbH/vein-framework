#include "vn_protocolevent.h"

namespace VeinNet
{
const int ProtocolEvent::m_registeredQEventType = QEvent::registerEventType();

ProtocolEvent::ProtocolEvent(EventOrigin t_fromLocalOrigin):
    QEvent(static_cast<QEvent::Type>(getQEventType())),
    m_localOrigin(static_cast<bool>(t_fromLocalOrigin))
{
    this->setAccepted(false);
}

int ProtocolEvent::getQEventType()
{
    return m_registeredQEventType;
}

QByteArray ProtocolEvent::buffer() const
{
    return m_flatBuffer;
}

void ProtocolEvent::setBuffer(QByteArray t_buffer)
{
    Q_ASSERT(t_buffer.isEmpty() == false);
    m_flatBuffer = t_buffer;
}

QList<QUuid> ProtocolEvent::receivers() const
{
    return m_receivers;
}

void ProtocolEvent::setReceivers(const QList<QUuid> &t_receivers)
{
    m_receivers = t_receivers;
}

bool ProtocolEvent::isOfLocalOrigin() const
{
    return m_localOrigin;
}

QUuid ProtocolEvent::peerId() const
{
    return m_peerId;
}

void ProtocolEvent::setPeerId(QUuid t_peerId)
{
    VF_ASSERT(t_peerId >= 0, "Peer id must be >= 0");
    m_peerId = t_peerId;
}

}
