#ifndef H2012_xiqnet_SERVER_PRIVATE_H
#define H2012_xiqnet_SERVER_PRIVATE_H

#include <QList>


class XiQNetPeer;
class XiQNetServer;

/**
 * @brief XiQNetServer PIMPL
 */
class XiQNetServerPrivate
{
  XiQNetServerPrivate(XiQNetServer *t_public);
  ~XiQNetServerPrivate();

  QList<XiQNetPeer*> m_clients;
  XiQNetServer *q_ptr = nullptr;

  friend class XiQNetServer;
};

#endif // H2012_xiqnet_SERVER_PRIVATE_H
