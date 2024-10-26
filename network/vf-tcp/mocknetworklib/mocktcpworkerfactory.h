#ifndef MOCKTCPWORKERFACTORY_H
#define MOCKTCPWORKERFACTORY_H

#include <abstracttcpworkerfactory.h>

namespace VeinTcp
{
class MockTcpWorkerFactory : public AbstractTcpWorkerFactory
{
public:
    static AbstractTcpWorkerFactoryPtr create();
    TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) override;
    TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) override;
};
}

#endif // MOCKTCPWORKERFACTORY_H
