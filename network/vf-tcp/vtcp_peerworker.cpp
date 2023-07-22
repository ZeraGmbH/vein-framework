#include "vtcp_peerworker.h"
#include "vtcp_peer.h"
#include <QDataStream>
#include <QTcpSocket>

namespace VeinTcp
{

TcpPeerWorker::TcpPeerWorker(TcpPeer *publicPeer) :
    q_ptr(publicPeer)
{
}

TcpPeerWorker::TcpPeerWorker(TcpPeer *publicPeer, qintptr socketDescriptor) :
    q_ptr(publicPeer)
{
    m_tcpSock = new QTcpSocket();
    connect(m_tcpSock, &QTcpSocket::connected, q_ptr, [this](){ emit q_ptr->sigConnectionEstablished(q_ptr); });
    connect(m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeerWorker::onReadyRead);
    connect(m_tcpSock, &QTcpSocket::disconnected, q_ptr, [this](){ emit q_ptr->sigConnectionClosed(q_ptr); });
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(
        m_tcpSock, &QTcpSocket::error, // Fix once using Qt > 5.14
        q_ptr, [this](QAbstractSocket::SocketError socketError) {
            emit q_ptr->emit sigSocketError(q_ptr, socketError);
        });
    connect(m_tcpSock, &QTcpSocket::disconnected, this, &TcpPeerWorker::closeConnection);
    if(m_tcpSock->setSocketDescriptor(socketDescriptor) == false) {
        emit q_ptr->sigSocketError(q_ptr, m_tcpSock->error());
        qFatal("[vein-tcp] Error setting clients socket descriptor");
    }
    m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

TcpPeerWorker::~TcpPeerWorker()
{
    // From the Qt manual QAbstractSocket::disconnected(): "Warning: If you need to delete
    // the sender() of this signal in a slot connected to it, use the deleteLater() function."
    // The destructor will be called in such a case so delete the QTcpSocket with deleteLater()
    m_tcpSock->deleteLater();
    m_tcpSock = nullptr;
}

void TcpPeerWorker::startConnection(QString ipAddress, quint16 port)
{
    //the tcp socket must not exist at this point
    Q_ASSERT_X(m_tcpSock==0, __PRETTY_FUNCTION__, "[vein-tcp] Do not re-use TcpPeer instances.");
    m_tcpSock = new QTcpSocket(q_ptr);
    connect(m_tcpSock, &QTcpSocket::connected, q_ptr, [this](){ emit q_ptr->sigConnectionEstablished(q_ptr); });
    connect(m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeerWorker::onReadyRead);
    connect(m_tcpSock, &QTcpSocket::disconnected, q_ptr, [this](){ emit q_ptr->sigConnectionClosed(q_ptr); });
    connect<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(
        m_tcpSock, &QTcpSocket::error, // Fix once using Qt > 5.14
        q_ptr, [this](QAbstractSocket::SocketError t_socketError){
            emit q_ptr->sigSocketError(q_ptr, t_socketError);
        });
    connect(m_tcpSock, &QTcpSocket::disconnected, this, &TcpPeerWorker::closeConnection);
    m_tcpSock->connectToHost(ipAddress, port);
    m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

bool TcpPeerWorker::isConnected() const
{
    return m_tcpSock &&
           (m_tcpSock->state() == QTcpSocket::ConnectedState || m_tcpSock->state()==QTcpSocket::BoundState);
}

QString TcpPeerWorker::getErrorString() const
{
    return m_tcpSock->errorString();
}

QByteArray TcpPeerWorker::readArray() const
{
    Q_ASSERT(m_tcpSock != nullptr && m_tcpSock->isOpen());
    QDataStream in(m_tcpSock);
    in.setVersion(QDataStream::Qt_5_7);
    in.startTransaction();
    QByteArray retVal;
    in >> retVal;

    if(in.commitTransaction() == true)
        return retVal;
    else { //need to wait for more data
        if(in.status() == QDataStream::ReadCorruptData)
            qWarning() << "[vein-tcp] Received corrupt data from client:" << q_ptr->getPeerId();
        return QByteArray();
    }

}

void TcpPeerWorker::sendArray(const QByteArray &byteArray) const
{
    Q_ASSERT(m_tcpSock != nullptr && m_tcpSock->isOpen());

    QDataStream out(m_tcpSock);
    out.setVersion(QDataStream::Qt_5_7);
    out << byteArray;

    if(out.status() == QDataStream::WriteFailed)
        qWarning() << "[vein-tcp] Write failed for client:" << q_ptr->getPeerId();
}

void TcpPeerWorker::onReadyRead()
{
    QByteArray newMessage;
    newMessage = readArray();
    while(!newMessage.isNull()) {
        emit q_ptr->sigMessageReceived(q_ptr, newMessage);
        newMessage = readArray();
    }
}

void TcpPeerWorker::closeConnection()
{
    Q_ASSERT(m_tcpSock);
    m_tcpSock->close();
}

}
