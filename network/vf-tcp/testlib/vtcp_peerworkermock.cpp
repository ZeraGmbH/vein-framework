#include "vtcp_peerworkermock.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkermock.h"

namespace VeinTcp
{

// client
TcpPeerWorkerMock::TcpPeerWorkerMock(TcpPeer *peer, secret) :
    m_peer(peer),
    m_bAmClientPeer(true),
    m_connectionEstablished(false)
{
}

// server
TcpPeerWorkerMock::TcpPeerWorkerMock(TcpPeer *peer, qintptr socketDescriptor, secret) :
    m_peer(peer),
    m_bAmClientPeer(false),
    m_clientPeer(reinterpret_cast<TcpPeer *>(socketDescriptor)),
    m_connectionEstablished(true)
{
}

// client only
void TcpPeerWorkerMock::startConnection(QString ipAddress, quint16 port)
{
    if(ipAddress != "localhost" && ipAddress != "127.0.0.1")
        emitSigSocketError(QAbstractSocket::HostNotFoundError);
    else {
        TcpServerWorkerMock* serverMock = TcpServerWorkerMock::getServerMock(port);
        if(!serverMock)
            emitSigSocketError(QAbstractSocket::ConnectionRefusedError);
        else {
            m_serverPeer = serverMock->emitSigClientConnected(m_peer);
            emitSigConnectionEstablished();
        }
    }
}

// client & server
void TcpPeerWorkerMock::sendArray(const QByteArray &byteArray) const
{
    Q_ASSERT_X(m_connectionEstablished, __PRETTY_FUNCTION__, "[vein-tcp] Trying to send data to disconnected host.");
    TcpPeerWorkerMock* const_this = const_cast<TcpPeerWorkerMock*>(this);
    if(m_bAmClientPeer)
        const_this->emitMessageReceived(m_serverPeer, byteArray);
    else
        const_this->emitMessageReceived(m_clientPeer, byteArray);
}

void TcpPeerWorkerMock::emitSigSocketError(QAbstractSocket::SocketError error)
{
    QMetaObject::invokeMethod(this,
                              "doEmitSigSocketError",
                              Qt::QueuedConnection,
                              Q_ARG(int, error));
}

void TcpPeerWorkerMock::doEmitSigSocketError(int error)
{
    emit m_peer->sigSocketError(m_peer, static_cast<QAbstractSocket::SocketError>(error));
}

// client only
void TcpPeerWorkerMock::emitSigConnectionEstablished()
{
    QMetaObject::invokeMethod(this,
                              "doEmitSigConnectionEstablished",
                              Qt::QueuedConnection);
}
void TcpPeerWorkerMock::doEmitSigConnectionEstablished()
{
    m_connectionEstablished = true;
    emit m_peer->sigConnectionEstablished(m_peer);
}

// client & server
void TcpPeerWorkerMock::emitMessageReceived(TcpPeer *peer, QByteArray message)
{
    QMetaObject::invokeMethod(this,
                              "doEmitMessageReceived",
                              Qt::QueuedConnection,
                              Q_ARG(VeinTcp::TcpPeer*, peer),
                              Q_ARG(QByteArray, message));
}
void TcpPeerWorkerMock::doEmitMessageReceived(TcpPeer *peer, QByteArray message)
{
    emit peer->sigMessageReceived(peer, message);
}

}
