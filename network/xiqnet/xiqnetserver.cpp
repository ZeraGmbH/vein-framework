#include "xiqnetserver.h"
#include "xiqnetserverprivate.h"
#include "xiqnetpeer.h"
#include <sys/socket.h>
#include <systemd/sd-daemon.h>

XiQNetServer::XiQNetServer(QObject *t_parent) :
    QTcpServer(t_parent),
    d_ptr(new XiQNetServerPrivate(this))
{
}

XiQNetServer::~XiQNetServer()
{
    delete d_ptr;
}

void XiQNetServer::setDefaultWrapper(XiQNetWrapper *t_wrapper)
{
    Q_ASSERT(t_wrapper != nullptr);
    d_ptr->m_defaultWrapper = t_wrapper;
}

void XiQNetServer::startServer(quint16 t_port, bool t_systemdSocket)
{
    if(t_systemdSocket) {
        //check if systemd has handed us a socket (service socket activation)
        int tmpSocketDescriptor = -1;
        const int socketCount=sd_listen_fds(0);
        for(int i=0; i<socketCount; ++i) {
            if(sd_is_socket_inet(SD_LISTEN_FDS_START+i, AF_UNSPEC, SOCK_STREAM, 1, t_port)) { //(int fd, int family, int type, int listening, uint16_t port)
                tmpSocketDescriptor = SD_LISTEN_FDS_START+i;
                break;
            }
        }
        if(tmpSocketDescriptor >= SD_LISTEN_FDS_START) {
            if(setSocketDescriptor(tmpSocketDescriptor))
                qDebug()<<"[xiqnet-qt] Inherited socket descriptor from systemd, listening on port:" << t_port;
        }
    }
    if(isListening() == false) {
        if(this->listen(QHostAddress::Any, t_port))
            qDebug()<<"[xiqnet-qt]Server Started on port:" << t_port;
        else
            qCritical() << "[xiqnet-qt]Server could not listen on port:" << t_port << "error:" << errorString();
    }
}

void XiQNetServer::clientDisconnectedSRV()
{
    Q_ASSERT(QObject::sender()!=nullptr); /// @todo redesign to not rely on QObject::sender
    XiQNetPeer *client = qobject_cast<XiQNetPeer*>(QObject::sender());
    Q_ASSERT(client != nullptr);

    d_ptr->m_clients.removeAll(client);
    ///@note use deletelater to execute other signal slot connections connected to the XiQNetPeer::sigConnectionClosed signal
    client->deleteLater();
}

void XiQNetServer::incomingConnection(qintptr t_socketDescriptor)
{
    qDebug()<<"[xiqnet-qt]Client connected";
    XiQNetPeer *client = new XiQNetPeer(t_socketDescriptor, this);
    if(d_ptr->m_defaultWrapper)
        client->setWrapper(d_ptr->m_defaultWrapper);
    d_ptr->m_clients.append(client);
    connect(client, &XiQNetPeer::sigConnectionClosed, this, &XiQNetServer::clientDisconnectedSRV);
    emit sigClientConnected(client);
}
