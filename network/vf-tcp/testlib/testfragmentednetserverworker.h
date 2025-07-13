#ifndef TESTFRAGMENTEDNETSERVERWORKER_H
#define TESTFRAGMENTEDNETSERVERWORKER_H

#include "vtcp_serverworker.h"

namespace VeinTcp
{
class TestFragmentedNetServerWorker : public TcpServerWorker
{
    Q_OBJECT
public:
    explicit TestFragmentedNetServerWorker(TcpServer *server, qint64 readBufferSize);
    TcpPeerWorkerInterfacePtr createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor) override;
private:
    qint64 m_readBufferSize;
};
}

#endif // TESTFRAGMENTEDNETSERVERWORKER_H
