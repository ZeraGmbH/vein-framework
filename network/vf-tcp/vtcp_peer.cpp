#include "vtcp_peer.h"
#include "vtcp_peerworker.h"
#include <QTcpSocket>
#include <QHostAddress>

namespace VeinTcp
{
TcpPeer::TcpPeer(QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new TcpPeerWorker(this))
{
}

TcpPeer::TcpPeer(qintptr t_socketDescriptor, QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new TcpPeerWorker(this, t_socketDescriptor))
{
}

void TcpPeer::startConnection(QString t_ipAddress, quint16 t_port)
{
    d_ptr->startConnection(t_ipAddress, t_port);
}

TcpPeer::~TcpPeer()
{
    delete d_ptr;
    d_ptr = nullptr;
}

QUuid TcpPeer::getPeerId() const
{
    return m_peerId;
}

void TcpPeer::setPeerId(QUuid t_peerId)
{
    Q_ASSERT(t_peerId.isNull() == false);
    m_peerId = t_peerId;
}

QString TcpPeer::getErrorString() const
{
    return d_ptr->getErrorString();
}

void TcpPeer::sendMessage(QByteArray t_message) const
{
    Q_ASSERT_X(d_ptr->isConnected(), __PRETTY_FUNCTION__, "[vein-tcp] Trying to send data to disconnected host.");
    d_ptr->sendArray(t_message);
}

}
