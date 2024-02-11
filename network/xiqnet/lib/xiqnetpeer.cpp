#include "xiqnetpeer.h"
#include "xiqnetpeerprivate.h"
#include "xiqnetwrapper.h"
#include <QTcpSocket>
#include <QHostAddress>

XiQNetPeer::XiQNetPeer(QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new XiQNetPeerPrivate(this))
{
}

XiQNetPeer::XiQNetPeer(qintptr t_socketDescriptor, QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new XiQNetPeerPrivate(this))
{
    d_ptr->m_tcpSock = new QTcpSocket();
    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, [this]() {
        emit sigConnectionEstablished(this);
    });
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, [this]() {
        emit sigConnectionClosed(this);
    });
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(d_ptr->m_tcpSock, &QTcpSocket::error, d_ptr->q_ptr, [this](QAbstractSocket::SocketError socketError) {
        emit d_ptr->q_ptr->sigSocketError(d_ptr->q_ptr, socketError);
    });
#else
    connect(d_ptr->m_tcpSock, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        emit sigSocketError(this, error);
    });
#endif

    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::closeConnection);
    if(!d_ptr->m_tcpSock->setSocketDescriptor(t_socketDescriptor)) {
        emit sigSocketError(this, d_ptr->m_tcpSock->error());
        qFatal("[xiqnet-qt] Error setting clients socket descriptor");
    }
    d_ptr->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

XiQNetPeer::~XiQNetPeer()
{
    //From the Qt manual QAbstractSocket::disconnected(): "Warning: If you need to delete the sender() of this signal in a slot connected to it, use the deleteLater() function."
    //This destructor will be called in such a case so delete the QTcpSocket with deleteLater()
    d_ptr->m_tcpSock->deleteLater();
    d_ptr->m_tcpSock=0;
    delete d_ptr;
    d_ptr=0;
}

QString XiQNetPeer::getIpAddress() const
{
    return d_ptr->m_tcpSock->peerAddress().toString();
}

quint16 XiQNetPeer::getPort() const
{
    return d_ptr->m_tcpSock->peerPort();
}

bool XiQNetPeer::isConnected() const
{
    return d_ptr->m_tcpSock && (d_ptr->m_tcpSock->state()==QTcpSocket::ConnectedState || d_ptr->m_tcpSock->state()==QTcpSocket::BoundState);
}

QUuid XiQNetPeer::getPeerId() const
{
    return d_ptr->m_peerId;
}

void XiQNetPeer::setPeerId(QUuid peerId)
{
    d_ptr->m_peerId = peerId;
}

void XiQNetPeer::writeRaw(QByteArray message) const
{
    d_ptr->m_tcpSock->write(message);
}

void XiQNetPeer::sendMessage(QByteArray message) const
{
    if(!isConnected()) {
        qWarning("[xiqnet-qt] Trying to send data to disconnected host.");
        return;
    }
    d_ptr->sendArray(message);
}

void XiQNetPeer::startConnection(QString t_ipAddress, quint16 t_port)
{
    //the tcp socket must not exist at this point
    Q_ASSERT_X(d_ptr->m_tcpSock==nullptr, __PRETTY_FUNCTION__, "[xiqnet-qt] Do not re-use XiQNetPeer instances, delete & recreate instead");

    d_ptr->m_tcpSock= new QTcpSocket(this);

    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, [this]() {
        emit sigConnectionEstablished(this);
    });
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, [this]() {
        emit sigConnectionClosed(this);
    });
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(d_ptr->m_tcpSock, &QTcpSocket::error, d_ptr->q_ptr, [this](QAbstractSocket::SocketError socketError) {
        emit d_ptr->q_ptr->sigSocketError(d_ptr->q_ptr, socketError);
    });
#else
    connect(d_ptr->m_tcpSock, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        emit sigSocketError(this, error);
    });
#endif
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::closeConnection);
    d_ptr->m_tcpSock->connectToHost(t_ipAddress, t_port);
    d_ptr->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

void XiQNetPeer::closeConnection()
{
    Q_ASSERT(d_ptr->m_tcpSock);

    d_ptr->m_tcpSock->close();
    //qDebug() << "disconnected";
}

void XiQNetPeer::onReadyRead()
{
    QByteArray newMessage;
    newMessage = d_ptr->readArray();
    while(!newMessage.isNull())
    {
        emit sigMessageReceived(this, newMessage);
        newMessage = d_ptr->readArray();
    }
}
