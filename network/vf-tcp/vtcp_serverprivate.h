#ifndef VEIN_TCP_SERVERPRIVATE_H
#define VEIN_TCP_SERVERPRIVATE_H

#include <QList>
#include <QTcpServer>

namespace VeinTcp
{
  class TcpPeer;
  class TcpServer;

  class TcpServerPrivate : public QTcpServer
  {
    TcpServerPrivate(TcpServer *server);
    ~TcpServerPrivate();
    void incomingConnection(qintptr socketDescriptor) override;
    bool startServer(quint16 port, bool systemdSocket);

    QList<TcpPeer*> m_clients;
    TcpServer *q_ptr = nullptr;

    friend class TcpServer;
  };
}
#endif // VEIN_TCP_SERVERPRIVATE_H
