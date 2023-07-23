#ifndef PROTOPEER_H
#define PROTOPEER_H

#include "xiqnet_global.h"
#include "xiqnet_export.h"
#include "xiqnetwrapper.h"
#include <QString>
#include <QTcpSocket>

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

    int getPeerId() const;
    void setPeerId(int t_peerId);

    // This is lazy crap
    QTcpSocket *getTcpSocket() const;
    // This makes the difference to vf-tcp: protobuf en/decoding is in here / vf-tcp
    // handles opaque QByteArray - see further 'google' below
    XiQNetWrapper *getWrapper() const;
    void setWrapper(XiQNetWrapper *value);
signals:
    void sigConnectionEstablished();
    void sigConnectionClosed();
    void sigMessageReceived(std::shared_ptr<google::protobuf::Message> t_Message);
    void sigSocketError(QAbstractSocket::SocketError t_socketError);
public slots:
    void sendMessage(const google::protobuf::Message &t_message) const;
    void startConnection(QString t_ipAddress, quint16 t_port);

private slots:
    void onReadyRead();
    void closeConnection();
private:
    XiQNetPeerPrivate *d_ptr = nullptr;
};

#endif // PROTOPEER_H
