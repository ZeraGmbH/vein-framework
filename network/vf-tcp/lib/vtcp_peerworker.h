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
private:
    // Just our friends can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    struct secret { explicit secret() = default; };
    friend class TcpNetworkFactory;
    friend class TcpServerWorker;
public:
    TcpPeerWorker(TcpPeer *peer, secret);
    TcpPeerWorker(TcpPeer *peer, qintptr socketDescriptor, secret);
    virtual ~TcpPeerWorker();

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
