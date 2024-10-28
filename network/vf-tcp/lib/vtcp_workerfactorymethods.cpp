#include "vtcp_workerfactorymethods.h"
#include "vtcp_peerworker.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{

std::function<TcpPeerWorkerInterfacePtr(TcpPeer *)> TcpNetworkFactoryMethods::m_createFunctionPeer = getCreateFunctionPeer();
std::function<TcpServerWorkerInterfacePtr(TcpServer *)> TcpNetworkFactoryMethods::m_createFunctionServer = getCreateFunctionServer();


TcpPeerWorkerInterfacePtr TcpNetworkFactoryMethods::createTcpPeerWorker(TcpPeer *peer)
{
    return m_createFunctionPeer(peer);
}

TcpServerWorkerInterfacePtr TcpNetworkFactoryMethods::createTcpServerWorker(TcpServer *server)
{
    return m_createFunctionServer(server);
}

std::function<TcpPeerWorkerInterfacePtr (TcpPeer *)> TcpNetworkFactoryMethods::getCreateFunctionPeer()
{
    return [](TcpPeer *peer) { return std::make_unique<TcpPeerWorker>(peer, TcpPeerWorker::secret()); };
}

std::function<TcpServerWorkerInterfacePtr (TcpServer *)> TcpNetworkFactoryMethods::getCreateFunctionServer()
{
    return [](TcpServer *server) { return std::make_unique<TcpServerWorker>(server, TcpServerWorker::secret()); };
}

}
