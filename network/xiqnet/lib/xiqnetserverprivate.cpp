#include "xiqnetserverprivate.h"
#include "xiqnetpeer.h"

XiQNetServerPrivate::XiQNetServerPrivate(XiQNetServer *t_public) : q_ptr(t_public)
{
}

XiQNetServerPrivate::~XiQNetServerPrivate()
{
  for(XiQNetPeer *tmpClient : m_clients)
  {
    tmpClient->deleteLater();
  }
  m_clients.clear();
}

