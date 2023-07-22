#include "vtcp_server.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{
TcpServer::TcpServer(QObject *parent) :
    QObject(parent),
    m_serverWorker(new TcpServerWorker(this))
{
}

TcpServer::~TcpServer()
{
    delete m_serverWorker;
}

bool TcpServer::startServer(quint16 port, bool systemdSocket)
{
    return m_serverWorker->startServer(port, systemdSocket);
}

bool TcpServer::isListening() const
{
    return m_serverWorker->isListening();
}
}
