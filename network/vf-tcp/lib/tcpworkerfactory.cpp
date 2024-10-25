#include "tcpworkerfactory.h"
#include "vtcp_peerworker.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{

TcpPeerWorkerInterfacePtr TcpWorkerFactory::createTcpPeerWorker(TcpPeer *peer)
{
    return std::make_unique<TcpPeerWorker>(peer, TcpPeerWorker::secret());
}

TcpServerWorkerInterfacePtr TcpWorkerFactory::createTcpServerWorker(TcpServer *server)
{
    return std::make_unique<TcpServerWorker>(server, TcpServerWorker::secret());
}

}
