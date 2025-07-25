#ifndef MOCKNETPEERWORKER_H
#define MOCKNETPEERWORKER_H

#include "vtcp_peerworkerinterface.h"
#include "mocknetserverworker.h"
#include <QAbstractSocket>

namespace VeinTcp
{
class TcpPeer;

class MockNetPeerWorker : public TcpPeerWorkerInterface
{
    Q_OBJECT
public:
    MockNetPeerWorker(TcpPeer *peer);
    MockNetPeerWorker(TcpPeer *peer, qintptr socketDescriptor);
    virtual ~MockNetPeerWorker();

    virtual QString getIpAddress() const override;
    virtual quint16 getPort() const override;
    bool isConnected() const override;
    void writeRaw(const QByteArray &message) const override;

    void startConnection(const QString &ipAddress, quint16 port) override;
    void sendArray(const QByteArray &byteArray) const override;

    QUuid getPeerId() const override;
    void setPeerId(const QUuid &peerId) override;

private slots:
    void doEmitSigConnectionEstablished();
private:
    void emitSigConnectionEstablished();
    void emitMessageReceived(QByteArray message) const;
    void emitSigSocketError(QAbstractSocket::SocketError error);

    QUuid m_peerId;
    bool m_amClientPeer;
    TcpPeer *m_myPeer = nullptr;
    TcpPeer *m_otherPeer = nullptr;
    bool m_connectionEstablished = false;
    QString m_ipAddress;
    quint16 m_port;
};
}
#endif // MOCKNETPEERWORKER_H
