#ifndef VEIN_TCP_PEER_H
#define VEIN_TCP_PEER_H

#include "vftcp_export.h"
#include <QString>
#include <QAbstractSocket>
#include <QUuid>

namespace VeinTcp
{
class TcpPeerPrivate;

class VFTCP_EXPORT TcpPeer : public QObject
{
    Q_OBJECT
public:
    explicit TcpPeer(QObject *t_parent = nullptr);
    explicit TcpPeer(qintptr t_socketDescriptor, QObject *t_parent = nullptr);
    ~TcpPeer();

    QUuid getPeerId() const;
    void setPeerId(QUuid t_peerId);

    QString getErrorString() const;
signals:
    void sigConnectionEstablished(TcpPeer *t_sender);
    void sigConnectionClosed(TcpPeer *t_sender);
    void sigMessageReceived(TcpPeer *t_sender, QByteArray t_message);
    void sigSocketError(TcpPeer *t_sender, QAbstractSocket::SocketError t_socketError);
public slots:
    void sendMessage(QByteArray t_message) const;
    void startConnection(QString t_ipAddress, quint16 t_port);

private slots:
    void onReadyRead();
    void closeConnection();
private:
    QUuid m_peerId;
    TcpPeerPrivate *d_ptr = nullptr;
};
}

#endif // VEIN_TCP_PEER_H
