#include "vtcp_server.h"
#include "vtcp_serverprivate.h"

#include "vtcp_peer.h"
#include <sys/socket.h>
#include <systemd/sd-daemon.h>

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

QList<TcpPeer *> TcpServer::getClientList() const
{
    return d_ptr->m_clients;
}

bool TcpServer::startServer(quint16 port, bool systemdSocket)
{
    bool retVal = false;
    if(systemdSocket) {
        //check if systemd has handed us a socket (service socket activation)
        int tmpSocketDescriptor = -1;
        const int socketCount=sd_listen_fds(0);
        for(int i=0; i<socketCount; ++i) {
            if(sd_is_socket_inet(SD_LISTEN_FDS_START+i, AF_UNSPEC, SOCK_STREAM, 1, port)) { //(int fd, int family, int type, int listening, uint16_t port)
                tmpSocketDescriptor = SD_LISTEN_FDS_START+i;
                break;
            }
        }

        if(tmpSocketDescriptor >= SD_LISTEN_FDS_START) {
            if(d_ptr->setSocketDescriptor(tmpSocketDescriptor))
            {
                retVal = true;
                qDebug()<<"[vein-tcp] Inherited socket descriptor from systemd, listening on port:" << port;
            }
        }
    }

    if(d_ptr->isListening() == false) {
        if(d_ptr->listen(QHostAddress::Any, port)) {
            retVal = true;
            qDebug()<<"[vein-tcp] Server Started on port:" << port;
        }
        else
            qCritical() << "[vein-tcp] Server could not listen on port:" << port << "error:" << d_ptr->errorString();
    }
    return retVal;
}

void TcpServer::clientDisconnectedSRV(TcpPeer *peer)
{
    d_ptr->m_clients.removeAll(peer);
    ///@note use deletelater to execute other signal slot connections connected to the TcpPeer::sigConnectionClosed signal
    peer->deleteLater();
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"[vein-tcp] Client connected";
    TcpPeer *client = new TcpPeer(socketDescriptor, this); //deleted in TcpServer::clientDisconnectedSRV
    d_ptr->m_clients.append(client);
    connect(client, &TcpPeer::sigConnectionClosed, this, &TcpServer::clientDisconnectedSRV);
    emit sigClientConnected(client);
}

bool TcpServer::isListening() const
{
    return d_ptr->isListening();
}
}
