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
private:
    // Just our friend MockNetTcpWorkerFactory can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    struct secret { explicit secret() = default; };
    friend class MockNetTcpWorkerFactory;
    friend class MockTcpWorkerFactory;
public:
    MockNetPeerWorker(TcpPeer *peer, secret);
    MockNetPeerWorker(TcpPeer *peer, qintptr socketDescriptor, secret);
    virtual ~MockNetPeerWorker();

    virtual QString getIpAddress() const override;
    virtual quint16 getPort() const override;
    bool isConnected() const override;
    void writeRaw(QByteArray message) const override;

    void startConnection(QString ipAddress, quint16 port) override;
    void sendArray(const QByteArray &byteArray) const override;

    QUuid getPeerId() const override;
    void setPeerId(QUuid peerId) override;

private slots:
    void doEmitSigConnectionEstablished();
private:
    void emitSigConnectionEstablished();
    void emitMessageReceived(VeinTcp::TcpPeer *peer, QByteArray message);
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
