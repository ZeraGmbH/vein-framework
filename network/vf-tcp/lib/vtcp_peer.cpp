#include "vtcp_peer.h"
#include "vtcp_workerfactorymethods.h"
#include <QTcpSocket>
#include <QHostAddress>

namespace VeinTcp
{
TcpPeer::TcpPeer(QObject *t_parent) :
    QObject(t_parent),
    m_worker(TcpWorkerFactoryMethods::createTcpPeerWorker(this))
{
}

TcpPeer::TcpPeer(qintptr t_socketDescriptor, QObject *t_parent) :
    QObject(t_parent),
    m_worker(TcpWorkerFactoryMethods::createTcpPeerWorker(this, t_socketDescriptor))
{
}

void TcpPeer::startConnection(QString t_ipAddress, quint16 t_port)
{
    m_worker->startConnection(t_ipAddress, t_port);
}

QUuid TcpPeer::getPeerId() const
{
    return m_worker->getPeerId();
}

void TcpPeer::setPeerId(QUuid t_peerId)
{
    m_worker->setPeerId(t_peerId);
}

QString TcpPeer::getErrorString() const
{
    return m_worker->getErrorString();
}

void TcpPeer::sendMessage(QByteArray t_message) const
{
    m_worker->sendArray(t_message);
}

}
