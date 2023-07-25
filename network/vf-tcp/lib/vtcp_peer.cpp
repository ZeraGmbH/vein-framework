#include "vtcp_peer.h"
#include "vtcp_workerfactorymethods.h"
#include <QTcpSocket>
#include <QHostAddress>

namespace VeinTcp
{
TcpPeer::TcpPeer(QObject *parent) :
    QObject(parent),
    m_worker(TcpWorkerFactoryMethods::createTcpPeerWorker(this))
{
}

TcpPeer::TcpPeer(qintptr socketDescriptor, QObject *parent) :
    QObject(parent),
    m_worker(TcpWorkerFactoryMethods::createTcpPeerWorker(this, socketDescriptor))
{
}

void TcpPeer::startConnection(QString ipAddress, quint16 port)
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

void TcpPeer::sendMessage(QByteArray message) const
{
    m_worker->sendArray(message);
}

}
