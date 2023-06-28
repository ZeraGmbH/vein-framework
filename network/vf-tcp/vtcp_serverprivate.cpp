#include "vtcp_serverprivate.h"
#include "vtcp_peer.h"

namespace VeinTcp
{
  TcpServerPrivate::TcpServerPrivate(TcpServer *t_public) : q_ptr(t_public)
  {
  }

  TcpServerPrivate::~TcpServerPrivate()
  {
    for(TcpPeer *tmpClient : m_clients)
    {
      tmpClient->deleteLater();
    }
    m_clients.clear();
  }
}
