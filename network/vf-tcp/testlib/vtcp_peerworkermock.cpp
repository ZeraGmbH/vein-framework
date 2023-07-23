#include "vtcp_peerworkermock.h"
#include "vtcp_peer.h"
#include <QDataStream>
#include <QTcpSocket>

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
}

QString TcpPeerWorkerMock::getErrorString() const
{
}

void TcpPeerWorkerMock::sendArray(const QByteArray &byteArray) const
{
}


}
