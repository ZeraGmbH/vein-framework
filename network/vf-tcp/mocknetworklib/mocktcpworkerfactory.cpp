#include "mocktcpworkerfactory.h"
#include "mocknetpeerworker.h"
#include "mocknetserverworker.h"

namespace VeinTcp
{
TcpPeerWorkerInterfacePtr MockTcpWorkerFactory::createPeerWorkerForClient(TcpPeer *peer)
{
    return std::make_unique<MockNetPeerWorker>(peer, MockNetPeerWorker::secret());
}

TcpServerWorkerInterfacePtr MockTcpWorkerFactory::createWorkerForServer(TcpServer *server)
{
    return std::make_unique<MockNetServerWorker>(server, MockNetServerWorker::secret());
}

}
