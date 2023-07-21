#include "vtcp_serverprivate.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"

namespace VeinTcp
{
TcpServerPrivate::TcpServerPrivate(TcpServer *t_public) : q_ptr(t_public)
{
}

TcpServerPrivate::~TcpServerPrivate()
{
    for(TcpPeer *tmpClient : m_clients)
        tmpClient->deleteLater();
    m_clients.clear();
}

void TcpServerPrivate::incomingConnection(qintptr t_socketDescriptor)
{
    q_ptr->incomingConnection(t_socketDescriptor);
}
}
