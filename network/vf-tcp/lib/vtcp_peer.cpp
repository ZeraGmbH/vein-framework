#include "vtcp_peer.h"
#include <QTcpSocket>
#include <QHostAddress>

namespace VeinTcp
{
int TcpPeer::m_instanceCount = 0;


TcpPeer::TcpPeer(AbstractTcpNetworkFactoryPtr tcpNetworkFactory, QObject *parent) :
    QObject(parent),
    m_worker(tcpNetworkFactory->createTcpPeerWorker(this))
{
    m_instanceCount++;
}

TcpPeer::TcpPeer(qintptr socketDescriptor, TcpServerWorkerInterface *serverWorker, QObject *parent) :
    QObject(parent),
    m_worker(serverWorker->createServerPeerWorker(this, socketDescriptor))
{
    m_instanceCount++;
}

TcpPeer::~TcpPeer()
{
    m_instanceCount--;
}

int TcpPeer::getInstanceCount()
{
    return m_instanceCount;
}

QString TcpPeer::getIpAddress() const
{
    return m_worker->getIpAddress();
}

quint16 TcpPeer::getPort() const
{
    return m_worker->getPort();
}

bool TcpPeer::isConnected() const
{
    return m_worker->isConnected();
}

void TcpPeer::startConnection(const QString &ipAddress, quint16 port)
{
    m_worker->startConnection(ipAddress, port);
}

QUuid TcpPeer::getPeerId() const
{
    return m_worker->getPeerId();
}

void TcpPeer::setPeerId(QUuid peerId)
{
    m_worker->setPeerId(peerId);
}

void TcpPeer::writeRaw(const QByteArray &message) const
{
    m_worker->writeRaw(message);
}

void TcpPeer::sendMessage(const QByteArray &message) const
{
    m_worker->sendArray(message);
}

}
