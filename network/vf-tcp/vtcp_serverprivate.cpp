#include "vtcp_serverprivate.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"

namespace VeinTcp
{
TcpServerPrivate::TcpServerPrivate(TcpServer *server) : q_ptr(server)
{
}

TcpServerPrivate::~TcpServerPrivate()
{
    for(TcpPeer *tmpClient : m_clients)
        tmpClient->deleteLater();
    m_clients.clear();
}

void TcpServerPrivate::incomingConnection(qintptr socketDescriptor)
{
    q_ptr->incomingConnection(socketDescriptor);
}
}
