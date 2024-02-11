#ifndef PROTOPEER_H
#define PROTOPEER_H

#include "xiqnet_export.h"
#include "xiqnetwrapper.h"
#include <QString>
#include <QTcpSocket>
#include <QUuid>

class XiQNetPeerPrivate;

class XIQNET_EXPORT XiQNetPeer : public QObject
{
    Q_OBJECT
public:
    explicit XiQNetPeer(QObject *t_parent = nullptr);
    explicit XiQNetPeer(qintptr t_socketDescriptor, QObject *t_parent = nullptr);
    ~XiQNetPeer();

    QString getIpAddress() const;
    quint16 getPort() const;
    bool isConnected() const;

    QUuid getPeerId() const;
    void setPeerId(QUuid peerId);

    void writeRaw(QByteArray message) const;

signals:
    void sigConnectionEstablished(XiQNetPeer *thisPeer);
    void sigConnectionClosed(XiQNetPeer *thisPeer);
    void sigMessageReceived(XiQNetPeer *thisPeer, QByteArray message);
    void sigSocketError(XiQNetPeer *thisPeer, QAbstractSocket::SocketError t_socketError);
public slots:
    void sendMessage(QByteArray message) const;
    void startConnection(QString t_ipAddress, quint16 t_port);

private slots:
    void onReadyRead();
    void closeConnection();
private:
    XiQNetPeerPrivate *d_ptr = nullptr;
};

#endif // PROTOPEER_H
