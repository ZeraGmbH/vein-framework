#ifndef TESTFRAGMENTEDNETPEERWORKER_H
#define TESTFRAGMENTEDNETPEERWORKER_H

#include "vtcp_peerworker.h"
#include <timertemplateqt.h>

namespace VeinTcp
{

class TestFragmentedNetPeerWorker : public TcpPeerWorker
{
    Q_OBJECT
public:
    explicit TestFragmentedNetPeerWorker(TcpPeer *peer, qint64 writeSize);
    explicit TestFragmentedNetPeerWorker(TcpPeer *peer, qintptr socketDescriptor, qint64 writeSize);
    void sendArray(const QByteArray &byteArray) const override;

    static int getReadyReadCount();
    static void resetReadyReadCount();

private slots:
    void onBytesWritten(qint64 bytes);
    void handleReadyRead();
private:
    void sendFraction() const;
    void doConnections();
    qint64 m_writeSize;
    std::unique_ptr<qint64> m_pendingDataOnSocket;
    std::unique_ptr<QByteArray> m_pendingSendData;
    static int m_readyReadCount;
};

}

#endif // TESTFRAGMENTEDNETPEERWORKER_H
