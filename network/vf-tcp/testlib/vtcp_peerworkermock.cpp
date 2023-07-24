#include "vtcp_peerworkermock.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkermock.h"

namespace VeinTcp
{

TcpPeerWorkerMock::TcpPeerWorkerMock(TcpPeer *peer, secret) :
    m_peer(peer)
{
}

TcpPeerWorkerMock::TcpPeerWorkerMock(TcpPeer *peer, qintptr socketDescriptor, secret) :
    m_peer(peer)
{
}

void TcpPeerWorkerMock::startConnection(QString ipAddress, quint16 port)
{
    if(ipAddress != "localhost" && ipAddress != "127.0.0.1")
        emitSigSocketError(QAbstractSocket::HostNotFoundError);
    else {
        TcpServerWorkerMock *serverMock = TcpServerWorkerMock::getServerMock(port);
        if(!serverMock)
            emitSigSocketError(QAbstractSocket::ConnectionRefusedError);
        else
            serverMock->emitSigClientConnected(m_peer);
    }
}

void TcpPeerWorkerMock::sendArray(const QByteArray &byteArray) const
{
    Q_ASSERT_X(m_connectionEstablished, __PRETTY_FUNCTION__, "[vein-tcp] Trying to send data to disconnected host.");
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

void TcpPeerWorkerMock::emitSigConnectionEstablished()
{
    m_connectionEstablished = true;
    emit m_peer->sigConnectionEstablished(m_peer);
}


}
