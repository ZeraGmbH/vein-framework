#ifndef VEIN_TCP_PEER_H
#define VEIN_TCP_PEER_H

#include "vftcp_export.h"
#include "vtcp_peerworkerinterface.h"
#include "vtcp_serverworkerinterface.h"
#include <QString>
#include <QAbstractSocket>
#include <QUuid>

namespace VeinTcp
{
class VFTCP_EXPORT TcpPeer : public QObject
{
    Q_OBJECT
public:
    explicit TcpPeer(QObject *parent = nullptr);
    explicit TcpPeer(qintptr socketDescriptor, TcpServerWorkerInterface* serverWorker, QObject *parent = nullptr);

    QString getIpAddress() const;
    quint16 getPort() const;
    bool isConnected() const;

    QUuid getPeerId() const;
    void setPeerId(QUuid t_peerId);

    void writeRaw(QByteArray message) const;

signals:
    void sigConnectionEstablished(VeinTcp::TcpPeer *thisPeer);
    void sigConnectionClosed(VeinTcp::TcpPeer *thisPeer);
    void sigMessageReceived(VeinTcp::TcpPeer *thisPeer, QByteArray message);
    void sigSocketError(VeinTcp::TcpPeer *thisPeer, QAbstractSocket::SocketError socketError);
public slots:
    void sendMessage(QByteArray message) const;
    void startConnection(QString ipAddress, quint16 port);

private:
    TcpPeerWorkerInterfacePtr m_worker;
};
}

#endif // VEIN_TCP_PEER_H
