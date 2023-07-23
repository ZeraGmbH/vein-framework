#include "vtcp_workerfactorymethods.h"
#include "vtcp_peerworker.h"
#include "vtcp_serverworker.h"

namespace VeinTcp
{

std::function<TcpPeerWorkerInterfacePtr(TcpPeer *)> TcpWorkerFactoryMethods::m_createFunctionPeer = [](TcpPeer *peer) {
    return std::make_unique<TcpPeerWorker>(peer, TcpPeerWorker::secret());
};

std::function<TcpPeerWorkerInterfacePtr(TcpPeer *, qintptr)> TcpWorkerFactoryMethods::m_createFunctionPeerDescriptor = [](TcpPeer *peer, qintptr socketDescriptor) {
    return std::make_unique<TcpPeerWorker>(peer, socketDescriptor, TcpPeerWorker::secret());
};

std::function<TcpServerWorkerInterfacePtr(TcpServer *)> TcpWorkerFactoryMethods::m_createFunctionServer = [](TcpServer *server) {
    return std::make_unique<TcpServerWorker>(server, TcpServerWorker::secret());
};


TcpPeerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpPeerWorker(TcpPeer *peer)
{
    return m_createFunctionPeer(peer);
}

TcpPeerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpPeerWorker(TcpPeer *peer, qintptr socketDescriptor)
{
    return m_createFunctionPeerDescriptor(peer, socketDescriptor);
}

TcpServerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpServerWorker(TcpServer *server)
{
    return m_createFunctionServer(server);
}

}
