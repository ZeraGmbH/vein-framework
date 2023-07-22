#include "vtcp_server.h"
#include "vtcp_serverprivate.h"
#include "vtcp_peer.h"

namespace VeinTcp
{
TcpServer::TcpServer(QObject *parent) :
    QObject(parent),
    d_ptr(new TcpServerPrivate(this))
{
}

TcpServer::~TcpServer()
{
    delete d_ptr;
}

bool TcpServer::startServer(quint16 port, bool systemdSocket)
{
    return d_ptr->startServer(port, systemdSocket);
}

void TcpServer::clientDisconnectedSRV(TcpPeer *peer)
{
    d_ptr->m_clients.removeAll(peer);
    ///@note use deletelater to execute other signal slot connections connected to the TcpPeer::sigConnectionClosed signal
    peer->deleteLater();
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"[vein-tcp] Client connected";
    TcpPeer *client = new TcpPeer(socketDescriptor, this); //deleted in TcpServer::clientDisconnectedSRV
    d_ptr->m_clients.append(client);
    connect(client, &TcpPeer::sigConnectionClosed, this, &TcpServer::clientDisconnectedSRV);
    emit sigClientConnected(client);
}

bool TcpServer::isListening() const
{
    return d_ptr->isListening();
}
}
