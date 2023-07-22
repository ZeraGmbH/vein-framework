#include "vtcp_peerprivate.h"
#include <QDataStream>
#include <QTcpSocket>

namespace VeinTcp
{

TcpPeerPrivate::TcpPeerPrivate(TcpPeer *t_publicPeer) :  q_ptr(t_publicPeer)
{
}

bool TcpPeerPrivate::isConnected() const
{
    return m_tcpSock &&
           (m_tcpSock->state() == QTcpSocket::ConnectedState || m_tcpSock->state()==QTcpSocket::BoundState);
}

QByteArray TcpPeerPrivate::readArray() const
{
    Q_ASSERT(m_tcpSock != nullptr && m_tcpSock->isOpen());
    QDataStream in(m_tcpSock);
    in.setVersion(QDataStream::Qt_5_7);
    in.startTransaction();
    QByteArray retVal;
    in >> retVal;

    if(in.commitTransaction() == true)
        return retVal;
    else { //need to wait for more data
        if(in.status() == QDataStream::ReadCorruptData)
            qWarning() << "[vein-tcp] Received corrupt data from client:" << m_peerId;
        return QByteArray();
    }

}

void TcpPeerPrivate::sendArray(const QByteArray &t_byteArray) const
{
    Q_ASSERT(m_tcpSock != nullptr && m_tcpSock->isOpen());

    QDataStream out(m_tcpSock);
    out.setVersion(QDataStream::Qt_5_7);
    out << t_byteArray;

    if(out.status() == QDataStream::WriteFailed)
        qWarning() << "[vein-tcp] Write failed for client:" << m_peerId;
}
}
