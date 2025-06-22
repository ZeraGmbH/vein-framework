#ifndef VEIN_TCP_PEER_H
#define VEIN_TCP_PEER_H

#include "abstracttcpnetworkfactory.h"
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
    explicit TcpPeer(AbstractTcpNetworkFactoryPtr tcpNetworkFactory, QObject *parent = nullptr);
    explicit TcpPeer(qintptr socketDescriptor, TcpServerWorkerInterface* serverWorker, QObject *parent = nullptr);
    ~TcpPeer();
    static int getInstanceCount();

    QString getIpAddress() const;
    quint16 getPort() const;
    bool isConnected() const;

    QUuid getPeerId() const;
    void setPeerId(QUuid t_peerId);

    void writeRaw(const QByteArray &message) const;

signals:
    void sigConnectionEstablished(VeinTcp::TcpPeer *thisPeer);
    void sigConnectionClosed(VeinTcp::TcpPeer *thisPeer);
    void sigMessageReceived(VeinTcp::TcpPeer *thisPeer, QByteArray message);
    void sigSocketError(VeinTcp::TcpPeer *thisPeer, QAbstractSocket::SocketError socketError);
public slots:
    void sendMessage(const QByteArray &message) const;
    void startConnection(const QString &ipAddress, quint16 port);

private:
    TcpPeerWorkerInterfacePtr m_worker;
    static int m_instanceCount;
};
}

#endif // VEIN_TCP_PEER_H
