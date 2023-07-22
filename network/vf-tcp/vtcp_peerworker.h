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
    // Just our friend TcpWorkerFactoryMethods can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    struct secret { explicit secret() = default; };
    friend class TcpWorkerFactoryMethods;
public:
    TcpPeerWorker(TcpPeer *publicPeer, secret);
    TcpPeerWorker(TcpPeer *publicPeer, qintptr socketDescriptor, secret);
    virtual ~TcpPeerWorker();
    void startConnection(QString ipAddress, quint16 port) override;
    QString getErrorString() const override;
    void sendArray(const QByteArray &byteArray) const override;

private slots:
    void onReadyRead();
    void closeConnection();
private:
    bool isConnected() const;
    QByteArray readArray() const;
    QTcpSocket *m_tcpSock = nullptr;
    TcpPeer *q_ptr = nullptr;
};
}
#endif // VEIN_TCP_PEERPRIVATE_H
