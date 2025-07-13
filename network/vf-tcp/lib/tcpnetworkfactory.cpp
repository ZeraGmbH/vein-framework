#include "tcpnetworkfactory.h"
#include "vtcp_peerworker.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{

AbstractTcpNetworkFactoryPtr TcpNetworkFactory::create()
{
    return std::make_shared<TcpNetworkFactory>();
}

TcpPeerWorkerInterfacePtr TcpNetworkFactory::createTcpPeerWorker(TcpPeer *peer)
{
    return std::make_unique<TcpPeerWorker>(peer);
}

TcpServerWorkerInterfacePtr TcpNetworkFactory::createTcpServerWorker(TcpServer *server)
{
    return std::make_unique<TcpServerWorker>(server);
}

}
