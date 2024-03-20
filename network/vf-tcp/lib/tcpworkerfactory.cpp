#include "tcpworkerfactory.h"
#include "vtcp_peerworker.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{

TcpPeerWorkerInterfacePtr TcpWorkerFactory::createPeerWorkerForClient(TcpPeer *peer, NETWORK network)
{
    Q_UNUSED(network)
    return std::make_unique<TcpPeerWorker>(peer, TcpPeerWorker::secret());
}

TcpServerWorkerInterfacePtr TcpWorkerFactory::createWorkerForServer(TcpServer *server, NETWORK network)
{
    Q_UNUSED(network)
    return std::make_unique<TcpServerWorker>(server, TcpServerWorker::secret());
}

}
