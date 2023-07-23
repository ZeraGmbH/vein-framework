#include "vtcp_workerfactorymethods.h"
#include "vtcp_peerworker.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{

std::function<TcpPeerWorkerInterfacePtr(TcpPeer *)> TcpWorkerFactoryMethods::m_createFunctionPeer = getCreateFunctionPeer();
std::function<TcpPeerWorkerInterfacePtr(TcpPeer *, qintptr)> TcpWorkerFactoryMethods::m_createFunctionPeerWithDescriptor = getCreateFunctionPeerWithDescriptor();
std::function<TcpServerWorkerInterfacePtr(TcpServer *)> TcpWorkerFactoryMethods::m_createFunctionServer = getCreateFunctionServer();


TcpPeerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpPeerWorker(TcpPeer *peer)
{
    return m_createFunctionPeer(peer);
}

TcpPeerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpPeerWorker(TcpPeer *peer, qintptr socketDescriptor)
{
    return m_createFunctionPeerWithDescriptor(peer, socketDescriptor);
}

TcpServerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpServerWorker(TcpServer *server)
{
    return m_createFunctionServer(server);
}

std::function<TcpPeerWorkerInterfacePtr (TcpPeer *)> TcpWorkerFactoryMethods::getCreateFunctionPeer()
{
    return [](TcpPeer *peer) { return std::make_unique<TcpPeerWorker>(peer, TcpPeerWorker::secret()); };
}

std::function<TcpPeerWorkerInterfacePtr (TcpPeer *, qintptr)> TcpWorkerFactoryMethods::getCreateFunctionPeerWithDescriptor()
{
    return [](TcpPeer *peer, qintptr socketDescriptor) { return std::make_unique<TcpPeerWorker>(peer, socketDescriptor, TcpPeerWorker::secret()); };
}

std::function<TcpServerWorkerInterfacePtr (TcpServer *)> TcpWorkerFactoryMethods::getCreateFunctionServer()
{
    return [](TcpServer *server) { return std::make_unique<TcpServerWorker>(server, TcpServerWorker::secret()); };
}

}
