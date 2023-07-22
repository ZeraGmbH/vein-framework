#include "vtcp_peer.h"
#include "vtcp_peerprivate.h"
#include <QTcpSocket>
#include <QHostAddress>

namespace VeinTcp
{
TcpPeer::TcpPeer(QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new TcpPeerPrivate(this))
{
}

TcpPeer::TcpPeer(qintptr t_socketDescriptor, QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new TcpPeerPrivate(this, t_socketDescriptor))
{
    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, [this](){ emit sigConnectionEstablished(this); });
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, [this](){ emit sigConnectionClosed(this); });
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(d_ptr->m_tcpSock, &QTcpSocket::error, this, [this](QAbstractSocket::SocketError t_socketError){ emit sigSocketError(this, t_socketError); });

    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &TcpPeer::closeConnection);
    if(d_ptr->m_tcpSock->setSocketDescriptor(t_socketDescriptor) == false) {
        emit sigSocketError(this, d_ptr->m_tcpSock->error());
        qFatal("[vein-tcp] Error setting clients socket descriptor");
    }
    d_ptr->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

void TcpPeer::startConnection(QString t_ipAddress, quint16 t_port)
{
    d_ptr->startConnection(t_ipAddress, t_port);
    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, [this](){ emit sigConnectionEstablished(this); });
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, [this](){ emit sigConnectionClosed(this); });
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(d_ptr->m_tcpSock, &QTcpSocket::error, this, [this](QAbstractSocket::SocketError t_socketError){ emit sigSocketError(this, t_socketError); });
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &TcpPeer::closeConnection);
    d_ptr->m_tcpSock->connectToHost(t_ipAddress, t_port);
    d_ptr->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

TcpPeer::~TcpPeer()
{
    //From the Qt manual QAbstractSocket::disconnected(): "Warning: If you need to delete the sender() of this signal in a slot connected to it, use the deleteLater() function."
    //The destructor will be called in such a case so delete the QTcpSocket with deleteLater()
    d_ptr->m_tcpSock->deleteLater();
    d_ptr->m_tcpSock=0;
    delete d_ptr;
    d_ptr=0;
}

QUuid TcpPeer::getPeerId() const
{
    return m_peerId;
}

void TcpPeer::setPeerId(QUuid t_peerId)
{
    Q_ASSERT(t_peerId.isNull() == false);

    m_peerId = t_peerId;
}

QString TcpPeer::getErrorString() const
{
    return d_ptr->m_tcpSock->errorString();
}

void TcpPeer::sendMessage(QByteArray t_message) const
{
    Q_ASSERT_X(d_ptr->isConnected(), __PRETTY_FUNCTION__, "[vein-tcp] Trying to send data to disconnected host.");
    d_ptr->sendArray(t_message);
}

void TcpPeer::closeConnection()
{
    Q_ASSERT(d_ptr->m_tcpSock);

    d_ptr->m_tcpSock->close();
    //qDebug() << "disconnected";
}

void TcpPeer::onReadyRead()
{
    QByteArray newMessage;
    newMessage = d_ptr->readArray();
    while(!newMessage.isNull())
    {
        //qDebug() << "[vein-tcp] Message received: "<<newMessage.toBase64();
        emit sigMessageReceived(this, newMessage);
        newMessage = d_ptr->readArray();
    }
}
}
