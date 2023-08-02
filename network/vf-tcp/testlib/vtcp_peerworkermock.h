#ifndef VEIN_TCP_PEERWORKERMOCK_H
#define VEIN_TCP_PEERWORKERMOCK_H

#include "vtcp_peerworkerinterface.h"
#include "vtcp_serverworkermock.h"
#include <QAbstractSocket>

namespace VeinTcp
{
class TcpPeer;

class TcpPeerWorkerMock : public TcpPeerWorkerInterface
{
    Q_OBJECT
private:
    // Just our friend TcpWorkerFactoryMethodsTest can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    struct secret { explicit secret() = default; };
    friend class TcpWorkerFactoryMethodsTest;
public:
    TcpPeerWorkerMock(TcpPeer *peer, secret);
    TcpPeerWorkerMock(TcpPeer *peer, qintptr socketDescriptor, secret);
    virtual ~TcpPeerWorkerMock();
    void startConnection(QString ipAddress, quint16 port) override;
    void sendArray(const QByteArray &byteArray) const override;
private slots:
    void doEmitSigConnectionEstablished();
private:
    void emitSigConnectionEstablished();
    void emitMessageReceived(VeinTcp::TcpPeer *peer, QByteArray message);
    void emitSigSocketError(QAbstractSocket::SocketError error);

    bool m_bAmClientPeer;
    TcpPeer *m_myPeer = nullptr;
    TcpPeer *m_otherPeer = nullptr;
    bool m_connectionEstablished = false;
};
}
#endif // VEIN_TCP_PEERWORKERMOCK_H
