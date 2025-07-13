#include "mocktcpnetworkfactory.h"
#include "mocknetpeerworker.h"
#include "mocknetserverworker.h"

namespace VeinTcp
{
AbstractTcpNetworkFactoryPtr MockTcpNetworkFactory::create()
{
    return std::make_shared<MockTcpNetworkFactory>();
}

TcpPeerWorkerInterfacePtr MockTcpNetworkFactory::createTcpPeerWorker(TcpPeer *peer)
{
    return std::make_unique<MockNetPeerWorker>(peer);
}

TcpServerWorkerInterfacePtr MockTcpNetworkFactory::createTcpServerWorker(TcpServer *server)
{
    return std::make_unique<MockNetServerWorker>(server);
}

}
