#include "vtcp_server.h"
#include "vtcp_workerfactorymethods.h"

namespace VeinTcp
{
TcpServer::TcpServer(AbstractTcpWorkerFactoryPtr tcpWorkerFactory, QObject *parent) :
    QObject(parent),
    m_serverWorker(tcpWorkerFactory->createTcpServerWorker(this))
{
}

TcpServer::TcpServer(QObject *parent) :
    QObject(parent),
    m_serverWorker(TcpWorkerFactoryMethods::createTcpServerWorker(this))
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
