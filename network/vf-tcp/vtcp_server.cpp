#include "vtcp_server.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{
TcpServer::TcpServer(QObject *parent) :
    QObject(parent),
    m_serverWorker(std::make_unique<TcpServerWorker>(this))
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
