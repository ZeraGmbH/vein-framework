#include "vtcp_serverprivate.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"
#include <systemd/sd-daemon.h>

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

bool TcpServerPrivate::startServer(quint16 port, bool systemdSocket)
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
            if(setSocketDescriptor(tmpSocketDescriptor)) {
                retVal = true;
                qDebug()<<"[vein-tcp] Inherited socket descriptor from systemd, listening on port:" << port;
            }
        }
    }

    if(isListening() == false) {
        if(listen(QHostAddress::Any, port)) {
            retVal = true;
            qDebug()<<"[vein-tcp] Server Started on port:" << port;
        }
        else
            qCritical() << "[vein-tcp] Server could not listen on port:" << port << "error:" << errorString();
    }
    return retVal;

}
}
