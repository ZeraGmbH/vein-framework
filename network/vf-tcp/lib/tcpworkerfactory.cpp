#include "tcpworkerfactory.h"
#include "vtcp_peerworker.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{

TcpPeerWorkerInterfacePtr TcpWorkerFactory::createPeerWorkerForClient(TcpPeer *peer)
{
    return std::make_unique<TcpPeerWorker>(peer, TcpPeerWorker::secret());
}

TcpServerWorkerInterfacePtr TcpWorkerFactory::createWorkerForServer(TcpServer *server)
{
    return std::make_unique<TcpServerWorker>(server, TcpServerWorker::secret());
}

}
