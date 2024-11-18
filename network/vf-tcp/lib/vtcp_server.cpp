#include "vtcp_server.h"

namespace VeinTcp
{
TcpServer::TcpServer(AbstractTcpNetworkFactoryPtr tcpNetworkFactory, QObject *parent) :
    QObject(parent),
    m_serverWorker(tcpNetworkFactory->createTcpServerWorker(this))
{
}

bool TcpServer::startServer(quint16 port, bool systemdSocket)
{
    return m_serverWorker->startServer(port, systemdSocket);
}

bool TcpServer::isListening() const
{
    return m_serverWorker->isListenActive();
}
}
