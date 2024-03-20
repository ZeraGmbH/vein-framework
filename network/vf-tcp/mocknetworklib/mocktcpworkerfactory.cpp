#include "mocktcpworkerfactory.h"
#include "mocknetpeerworker.h"
#include "mocknetserverworker.h"

namespace VeinTcp
{
TcpPeerWorkerInterfacePtr MockTcpWorkerFactory::createPeerWorkerForClient(TcpPeer *peer, NETWORK network)
{
    Q_UNUSED(network)
    return std::make_unique<MockNetPeerWorker>(peer, MockNetPeerWorker::secret());
}

TcpServerWorkerInterfacePtr MockTcpWorkerFactory::createWorkerForServer(TcpServer *server, NETWORK network)
{
    Q_UNUSED(network)
    return std::make_unique<MockNetServerWorker>(server, MockNetServerWorker::secret());
}

}
