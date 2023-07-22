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

bool TcpServer::isListening() const
{
    return d_ptr->isListening();
}
}
