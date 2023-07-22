#include "vtcp_peerprivate.h"
#include "vtcp_peer.h"
#include <QDataStream>
#include <QTcpSocket>

namespace VeinTcp
{

TcpPeerPrivate::TcpPeerPrivate(TcpPeer *publicPeer) :
    q_ptr(publicPeer)
{
}

TcpPeerPrivate::TcpPeerPrivate(TcpPeer *publicPeer, qintptr socketDescriptor) :
    q_ptr(publicPeer)
{
    m_tcpSock = new QTcpSocket();
    connect(m_tcpSock, &QTcpSocket::connected, q_ptr, [this](){ emit q_ptr->sigConnectionEstablished(q_ptr); });
    connect(m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeerPrivate::onReadyRead);

}

void TcpPeerPrivate::startConnection(QString ipAddress, quint16 port)
{
    //the tcp socket must not exist at this point
    Q_ASSERT_X(m_tcpSock==0, __PRETTY_FUNCTION__, "[vein-tcp] Do not re-use TcpPeer instances.");
    m_tcpSock = new QTcpSocket(q_ptr);
    connect(m_tcpSock, &QTcpSocket::connected, q_ptr, [this](){ emit q_ptr->sigConnectionEstablished(q_ptr); });
    connect(m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeerPrivate::onReadyRead);

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
            qWarning() << "[vein-tcp] Received corrupt data from client:" << q_ptr->getPeerId();
        return QByteArray();
    }

}

void TcpPeerPrivate::sendArray(const QByteArray &byteArray) const
{
    Q_ASSERT(m_tcpSock != nullptr && m_tcpSock->isOpen());

    QDataStream out(m_tcpSock);
    out.setVersion(QDataStream::Qt_5_7);
    out << byteArray;

    if(out.status() == QDataStream::WriteFailed)
        qWarning() << "[vein-tcp] Write failed for client:" << q_ptr->getPeerId();
}

void TcpPeerPrivate::onReadyRead()
{
    QByteArray newMessage;
    newMessage = readArray();
    while(!newMessage.isNull())
    {
        emit q_ptr->sigMessageReceived(q_ptr, newMessage);
        newMessage = readArray();
    }
}

}
