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
    d_ptr->m_wrapper = nullptr;
    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, &XiQNetPeer::sigConnectionEstablished);
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::sigConnectionClosed);
    connect(d_ptr->m_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));

    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::closeConnection);
    if(!d_ptr->m_tcpSock->setSocketDescriptor(t_socketDescriptor)) {
        emit sigSocketError(d_ptr->m_tcpSock->error());
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

int XiQNetPeer::getPeerId() const
{
    return d_ptr->m_peerId;
}

void XiQNetPeer::setPeerId(int t_peerId)
{
    Q_ASSERT(t_peerId>=0);

    d_ptr->m_peerId = t_peerId;
}

QTcpSocket *XiQNetPeer::getTcpSocket() const
{
    return d_ptr->m_tcpSock;
}

XiQNetWrapper *XiQNetPeer::getWrapper() const
{
    return d_ptr->m_wrapper;
}

void XiQNetPeer::setWrapper(XiQNetWrapper *value)
{
    d_ptr->m_wrapper = value;
}

void XiQNetPeer::sendMessage(const google::protobuf::Message &t_message) const
{
    if(!isConnected() || !d_ptr->m_wrapper) {
        qWarning("[xiqnet-qt] Trying to send data to disconnected host.");
        return;
    }
    d_ptr->sendArray(d_ptr->m_wrapper->protobufToByteArray(t_message));
}

void XiQNetPeer::startConnection(QString t_ipAddress, quint16 t_port)
{
    //the tcp socket must not exist at this point
    Q_ASSERT_X(d_ptr->m_tcpSock==nullptr, __PRETTY_FUNCTION__, "[xiqnet-qt] Do not re-use XiQNetPeer instances, delete & recreate instead");

    d_ptr->m_tcpSock= new QTcpSocket(this);

    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, &XiQNetPeer::sigConnectionEstablished);
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::sigConnectionClosed);
    connect(d_ptr->m_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::closeConnection);
    d_ptr->m_tcpSock->connectToHost(t_ipAddress, t_port);
    d_ptr->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

void XiQNetPeer::closeConnection()
{
    Q_ASSERT(d_ptr->m_tcpSock);

    //void out the wrapper
    d_ptr->m_wrapper=0;

    d_ptr->m_tcpSock->close();
    //qDebug() << "disconnected";
}

void XiQNetPeer::onReadyRead()
{
    Q_ASSERT(d_ptr->m_wrapper != nullptr);

    QByteArray newMessage;
    newMessage = d_ptr->readArray();
    while(!newMessage.isNull())
    {
        //qDebug() << "[proto-net-qt] Message received: "<<newMessage.toBase64();
        emit sigMessageReceived(d_ptr->m_wrapper->byteArrayToProtobuf(newMessage));
        newMessage = d_ptr->readArray();
    }
}
