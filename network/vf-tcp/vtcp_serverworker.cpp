#include "vtcp_serverworker.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"
#include <systemd/sd-daemon.h>

namespace VeinTcp
{
TcpServerWorker::TcpServerWorker(TcpServer *server) :
    m_server(server)
{
}

TcpServerWorker::~TcpServerWorker()
{
    for(TcpPeer *tmpClient : qAsConst(m_clients))
        tmpClient->deleteLater();
    m_clients.clear();
}

void TcpServerWorker::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"[vein-tcp] Client connected";
    TcpPeer *client = new TcpPeer(socketDescriptor, this); //deleted in TcpServer::clientDisconnectedSRV
    m_clients.append(client);
    connect(client, &TcpPeer::sigConnectionClosed, this, &TcpServerWorker::clientDisconnectedSRV);
    emit m_server->sigClientConnected(client);
}

void TcpServerWorker::clientDisconnectedSRV(TcpPeer *peer)
{
    m_clients.removeAll(peer);
    ///@note use deletelater to execute other signal slot connections connected to the TcpPeer::sigConnectionClosed signal
    peer->deleteLater();
}

bool TcpServerWorker::startServer(quint16 port, bool systemdSocket)
{
    bool retVal = false;
    if(systemdSocket) {
        //check if systemd has handed us a socket (service socket activation)
        int tmpSocketDescriptor = -1;
        const int socketCount = sd_listen_fds(0);
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
