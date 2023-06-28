#include "vtcp_server.h"
#include "vtcp_serverprivate.h"

#include "vtcp_peer.h"
#include <sys/socket.h>
#include <systemd/sd-daemon.h>

namespace VeinTcp
{
  TcpServer::TcpServer(QObject *t_parent) :
    QTcpServer(t_parent),
    d_ptr(new TcpServerPrivate(this))
  {
  }

  TcpServer::~TcpServer()
  {
    delete d_ptr;
  }

  QList<TcpPeer *> TcpServer::getClientList() const
  {
    return d_ptr->m_clients;
  }

  void TcpServer::broadcastMessage(QByteArray t_message) const
  {
    for(TcpPeer *tmpClient : d_ptr->m_clients)
    {
      tmpClient->sendMessage(t_message);
    }
  }

  bool TcpServer::startServer(quint16 t_port, bool t_systemdSocket)
  {
    bool retVal = false;

    if(t_systemdSocket) {
        //check if systemd has handed us a socket (service socket activation)
        int tmpSocketDescriptor = -1;
        const int socketCount=sd_listen_fds(0);

        for(int i=0; i<socketCount; ++i)
        {
          if(sd_is_socket_inet(SD_LISTEN_FDS_START+i, AF_UNSPEC, SOCK_STREAM, 1, t_port)) //(int fd, int family, int type, int listening, uint16_t port)
          {
            tmpSocketDescriptor = SD_LISTEN_FDS_START+i;
            break;
          }
        }

        if(tmpSocketDescriptor >= SD_LISTEN_FDS_START)
        {
          if(setSocketDescriptor(tmpSocketDescriptor))
          {
            retVal = true;
            qDebug()<<"[vein-tcp] Inherited socket descriptor from systemd, listening on port:" << t_port;
          }
        }
    }

    if(isListening() == false)
    {
      if(this->listen(QHostAddress::Any, t_port))
      {
        retVal = true;
        qDebug()<<"[vein-tcp] Server Started on port:" << t_port;
      }
      else
      {
        qCritical() << "[vein-tcp] Server could not listen on port:" << t_port << "error:" << errorString();
      }
    }
    return retVal;
  }

  void TcpServer::clientDisconnectedSRV(TcpPeer *t_peer)
  {
    d_ptr->m_clients.removeAll(t_peer);
    ///@note use deletelater to execute other signal slot connections connected to the TcpPeer::sigConnectionClosed signal
    t_peer->deleteLater();
  }

  void TcpServer::incomingConnection(qintptr t_socketDescriptor)
  {
    qDebug()<<"[vein-tcp] Client connected";

    TcpPeer *client = new TcpPeer(t_socketDescriptor, this); //deleted in TcpServer::clientDisconnectedSRV
    d_ptr->m_clients.append(client);
    connect(client, &TcpPeer::sigConnectionClosed, this, &TcpServer::clientDisconnectedSRV);
    emit sigClientConnected(client);
  }
}
