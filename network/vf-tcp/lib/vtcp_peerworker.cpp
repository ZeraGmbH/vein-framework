#include "vtcp_peerworker.h"
#include "vtcp_peer.h"
#include <QDataStream>
#include <QTcpSocket>
#include <QHostAddress>

namespace VeinTcp
{

// client side
TcpPeerWorker::TcpPeerWorker(TcpPeer *peer) :
    m_tcpSock(new QTcpSocket(peer)),
    m_peer(peer)
{
    prepareSocket();
}

// server side
TcpPeerWorker::TcpPeerWorker(TcpPeer *peer, qintptr socketDescriptor) :
    m_tcpSock(new QTcpSocket),
    m_peer(peer)
{
    prepareSocket();
    if (!m_tcpSock->setSocketDescriptor(socketDescriptor)) {
        emit m_peer->sigSocketError(m_peer, m_tcpSock->error());
        qFatal("[vein-tcp] Error setting clients socket descriptor");
    }
}

TcpPeerWorker::~TcpPeerWorker()
{
    // From the Qt manual QAbstractSocket::disconnected(): "Warning: If you need to delete
    // the sender() of this signal in a slot connected to it, use the deleteLater() function."
    // The destructor will be called in such a case so delete the QTcpSocket with deleteLater()
    m_tcpSock->deleteLater();
    m_tcpSock = nullptr;
}

QString TcpPeerWorker::getIpAddress() const
{
    return m_tcpSock->peerAddress().toString();
}

quint16 TcpPeerWorker::getPort() const
{
    return m_tcpSock->peerPort();
}

void VeinTcp::TcpPeerWorker::prepareSocket()
{
    connect(m_tcpSock, &QTcpSocket::connected, m_peer, [this](){ emit m_peer->sigConnectionEstablished(m_peer); });
    connect(m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeerWorker::onReadyRead);
    connect(m_tcpSock, &QTcpSocket::disconnected, m_peer, [this](){ emit m_peer->sigConnectionClosed(m_peer); });
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(m_tcpSock, &QTcpSocket::error, m_peer, [this](QAbstractSocket::SocketError socketError) {
        emit m_peer->sigSocketError(m_peer, socketError);
    });
#else
    connect(m_tcpSock, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        emit m_peer->sigSocketError(m_peer, error);
    });
#endif
    connect(m_tcpSock, &QTcpSocket::disconnected, this, &TcpPeerWorker::closeConnection);
    m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

void TcpPeerWorker::startConnection(const QString &ipAddress, quint16 port)
{
    m_tcpSock->connectToHost(ipAddress, port);
}

bool TcpPeerWorker::isConnected() const
{
    return m_tcpSock &&
           (m_tcpSock->state() == QTcpSocket::ConnectedState || m_tcpSock->state()==QTcpSocket::BoundState);
}

void TcpPeerWorker::writeRaw(const QByteArray &message) const
{
    m_tcpSock->write(message);
}

QByteArray TcpPeerWorker::readArray() const
{
    Q_ASSERT(m_tcpSock != nullptr && m_tcpSock->isOpen());
    QDataStream in(m_tcpSock);
    in.setVersion(QDataStream::Qt_5_7);
    in.startTransaction();
    QByteArray retVal;
    in >> retVal;

    if(in.commitTransaction())
        return retVal;
    else { //need to wait for more data
        if(in.status() == QDataStream::ReadCorruptData)
            qWarning() << "[vein-tcp] Received corrupt data from client:" << m_peer->getPeerId();
        return QByteArray();
    }

}

void TcpPeerWorker::sendArray(const QByteArray &byteArray) const
{
    if(!isConnected() || !m_tcpSock || !m_tcpSock->isOpen()) {
        qWarning("[vein-tcp] Trying to send data to disconnected host.");
        return;
    }
    QDataStream out(m_tcpSock);
    out.setVersion(QDataStream::Qt_5_7);
    out << byteArray;
    if(out.status() == QDataStream::WriteFailed)
        qWarning() << "[vein-tcp] Write failed for client:" << m_peer->getPeerId();
}

QUuid TcpPeerWorker::getPeerId() const
{
    return m_peerId;
}

void TcpPeerWorker::setPeerId(const QUuid &peerId)
{
    Q_ASSERT(!peerId.isNull());
    m_peerId = peerId;
}

void TcpPeerWorker::onReadyRead()
{
    QByteArray newMessage = readArray();
    while(!newMessage.isNull()) {
        emit m_peer->sigMessageReceived(m_peer, newMessage);
        newMessage = readArray();
    }
}

void TcpPeerWorker::closeConnection()
{
    Q_ASSERT(m_tcpSock);
    m_tcpSock->close();
}

}
