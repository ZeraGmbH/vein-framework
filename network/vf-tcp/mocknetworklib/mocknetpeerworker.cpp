#include "mocknetpeerworker.h"
#include "mocknetserverworker.h"
#include "vtcp_peer.h"

namespace VeinTcp
{

// client
MockNetPeerWorker::MockNetPeerWorker(TcpPeer *peer) :
    m_amClientPeer(true),
    m_myPeer(peer),
    m_connectionEstablished(false)
{
    connect(m_myPeer, &TcpPeer::sigConnectionClosed, this, [&](TcpPeer*){
        m_connectionEstablished = false;
    });
}

// server
MockNetPeerWorker::MockNetPeerWorker(TcpPeer *peer, qintptr socketDescriptor) :
    m_amClientPeer(false),
    m_myPeer(peer),
    m_otherPeer(reinterpret_cast<TcpPeer *>(socketDescriptor)),
    m_connectionEstablished(true)
{
    connect(m_myPeer, &TcpPeer::sigConnectionClosed, this, [&](TcpPeer*){
        m_connectionEstablished = false;
    });
}

// client only
void MockNetPeerWorker::startConnection(const QString &ipAddress, quint16 port)
{
    if(ipAddress != "localhost" && ipAddress != "127.0.0.1")
        emitSigSocketError(QAbstractSocket::HostNotFoundError);
    else {
        m_ipAddress = ipAddress; // correct only with checks above
        m_port = port;
        MockNetServerWorker* serverMock = MockNetServerWorker::getServerMock(port);
        if(!serverMock)
            emitSigSocketError(QAbstractSocket::ConnectionRefusedError);
        else {
            m_otherPeer = serverMock->emitSigClientConnected(m_myPeer);
            emitSigConnectionEstablished();
        }
    }
}

MockNetPeerWorker::~MockNetPeerWorker()
{
    if(m_connectionEstablished)
        emit m_myPeer->sigConnectionClosed(m_myPeer);
}

QString MockNetPeerWorker::getIpAddress() const
{
    return m_ipAddress;
}

quint16 MockNetPeerWorker::getPort() const
{
    return m_port;
}

bool MockNetPeerWorker::isConnected() const
{
    return m_connectionEstablished;
}

// client & server
void MockNetPeerWorker::writeRaw(const QByteArray &message) const
{
    emitMessageReceived(message);
}

void MockNetPeerWorker::sendArray(const QByteArray &byteArray) const
{
    emitMessageReceived(byteArray);
}

QUuid MockNetPeerWorker::getPeerId() const
{
    return m_peerId;
}

void MockNetPeerWorker::setPeerId(const QUuid &peerId)
{
    Q_ASSERT(!peerId.isNull());
    m_peerId = peerId;
}

void MockNetPeerWorker::emitSigSocketError(QAbstractSocket::SocketError error)
{
    QMetaObject::invokeMethod(m_myPeer,
                              "sigSocketError",
                              Qt::QueuedConnection,
                              Q_ARG(VeinTcp::TcpPeer*, m_myPeer),
                              Q_ARG(QAbstractSocket::SocketError, error));
}

// client only
void MockNetPeerWorker::emitSigConnectionEstablished()
{
    QMetaObject::invokeMethod(this,
                              "doEmitSigConnectionEstablished",
                              Qt::QueuedConnection);
}
void MockNetPeerWorker::doEmitSigConnectionEstablished()
{
    m_connectionEstablished = true;
    emit m_myPeer->sigConnectionEstablished(m_myPeer);
}

// client & server
void MockNetPeerWorker::emitMessageReceived(QByteArray message) const
{
    if(!m_otherPeer || !m_connectionEstablished) {
        qWarning("[vein-tcp] Trying to send data to disconnected host.");
        return;
    }
    QMetaObject::invokeMethod(m_otherPeer,
                              "sigMessageReceived",
                              Qt::QueuedConnection,
                              Q_ARG(VeinTcp::TcpPeer*, m_otherPeer),
                              Q_ARG(QByteArray, message));
}
}
