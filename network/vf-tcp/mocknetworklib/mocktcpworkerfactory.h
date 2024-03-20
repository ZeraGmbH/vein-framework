#ifndef MOCKTCPWORKERFACTORY_H
#define MOCKTCPWORKERFACTORY_H

#include <abstracttcpworkerfactory.h>

namespace VeinTcp
{
class MockTcpWorkerFactory : public AbstractTcpWorkerFactory
{
public:
    TcpPeerWorkerInterfacePtr createPeerWorkerForClient(TcpPeer *peer, NETWORK network) override;
    TcpServerWorkerInterfacePtr createWorkerForServer(TcpServer *server, NETWORK network) override;
};
}

#endif // MOCKTCPWORKERFACTORY_H
