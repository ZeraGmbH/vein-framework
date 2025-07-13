#ifndef VEIN_TCP_PEERPRIVATE_H
#define VEIN_TCP_PEERPRIVATE_H

#include "vtcp_peerworkerinterface.h"
#include <QTcpSocket>

namespace VeinTcp
{
class TcpPeer;

class TcpPeerWorker : public TcpPeerWorkerInterface
{
    Q_OBJECT
public:
    TcpPeerWorker(TcpPeer *peer);
    TcpPeerWorker(TcpPeer *peer, qintptr socketDescriptor);
    ~TcpPeerWorker() override;

    virtual QString getIpAddress() const override;
    virtual quint16 getPort() const override;
    bool isConnected() const override;
    void writeRaw(const QByteArray &message) const override;

    void startConnection(const QString &ipAddress, quint16 port) override;
    void sendArray(const QByteArray &byteArray) const override;
    QUuid getPeerId() const override;
    void setPeerId(const QUuid &peerId) override;

private slots:
    void onReadyRead();
    void closeConnection();
private:
    void prepareSocket();
    QByteArray readArray() const;
    QUuid m_peerId;
    QTcpSocket *m_tcpSock = nullptr;
    TcpPeer *m_peer = nullptr;
};
}
#endif // VEIN_TCP_PEERPRIVATE_H
