#ifndef MOCKTCPWORKERFACTORY_H
#define MOCKTCPWORKERFACTORY_H

#include <abstracttcpworkerfactory.h>

namespace VeinTcp
{
class MockTcpWorkerFactory : public AbstractTcpWorkerFactory
{
public:
    TcpPeerWorkerInterfacePtr createPeerWorkerForClient(TcpPeer *peer) override;
    TcpServerWorkerInterfacePtr createWorkerForServer(TcpServer *server) override;
};
}

#endif // MOCKTCPWORKERFACTORY_H
