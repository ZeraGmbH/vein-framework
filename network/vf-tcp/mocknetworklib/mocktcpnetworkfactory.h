#ifndef MOCKTCPWORKERFACTORY_H
#define MOCKTCPWORKERFACTORY_H

#include <abstracttcpnetworkfactory.h>

namespace VeinTcp
{
class MockTcpNetworkFactory : public AbstractTcpNetworkFactory
{
public:
    static AbstractTcpNetworkFactoryPtr create();
    TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) override;
    TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) override;
};
}

#endif // MOCKTCPWORKERFACTORY_H
