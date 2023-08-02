#include "vtcp_peerworkermock.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkermock.h"

namespace VeinTcp
{

// client
TcpPeerWorkerMock::TcpPeerWorkerMock(TcpPeer *peer, secret) :
    m_bAmClientPeer(true),
    m_myPeer(peer),
    m_connectionEstablished(false)
{
    connect(m_myPeer, &TcpPeer::sigConnectionClosed, this, [&](TcpPeer*){
        m_connectionEstablished = false;
    });
}

// server
TcpPeerWorkerMock::TcpPeerWorkerMock(TcpPeer *peer, qintptr socketDescriptor, secret) :
    m_bAmClientPeer(false),
    m_myPeer(peer),
    m_otherPeer(reinterpret_cast<TcpPeer *>(socketDescriptor)),
    m_connectionEstablished(true)
{
    connect(m_myPeer, &TcpPeer::sigConnectionClosed, this, [&](TcpPeer*){
        m_connectionEstablished = false;
    });
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
            m_otherPeer = serverMock->emitSigClientConnected(m_myPeer);
            emitSigConnectionEstablished();
        }
    }
}

TcpPeerWorkerMock::~TcpPeerWorkerMock()
{
    if(m_connectionEstablished)
        emit m_myPeer->sigConnectionClosed(m_myPeer);
}

// client & server
void TcpPeerWorkerMock::sendArray(const QByteArray &byteArray) const
{
    Q_ASSERT_X(m_connectionEstablished, __PRETTY_FUNCTION__, "[vein-tcp] Trying to send data to disconnected host.");
    TcpPeerWorkerMock* const_this = const_cast<TcpPeerWorkerMock*>(this);
    const_this->emitMessageReceived(m_otherPeer, byteArray);
}

void TcpPeerWorkerMock::emitSigSocketError(QAbstractSocket::SocketError error)
{
    QMetaObject::invokeMethod(m_myPeer,
                              "sigSocketError",
                              Qt::QueuedConnection,
                              Q_ARG(VeinTcp::TcpPeer*, m_myPeer),
                              Q_ARG(QAbstractSocket::SocketError, error));
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
    emit m_myPeer->sigConnectionEstablished(m_myPeer);
}

// client & server
void TcpPeerWorkerMock::emitMessageReceived(TcpPeer *peer, QByteArray message)
{
    QMetaObject::invokeMethod(peer,
                              "sigMessageReceived",
                              Qt::QueuedConnection,
                              Q_ARG(VeinTcp::TcpPeer*, peer),
                              Q_ARG(QByteArray, message));
}
}
