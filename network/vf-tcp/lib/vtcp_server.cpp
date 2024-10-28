#include "vtcp_server.h"
#include "vtcp_workerfactorymethods.h"

namespace VeinTcp
{
TcpServer::TcpServer(AbstractTcpNetworkFactoryPtr tcpNetworkFactory, QObject *parent) :
    QObject(parent),
    m_serverWorker(tcpNetworkFactory->createTcpServerWorker(this))
{
}

TcpServer::TcpServer(QObject *parent) :
    QObject(parent),
    m_serverWorker(TcpNetworkFactoryMethods::createTcpServerWorker(this))
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
