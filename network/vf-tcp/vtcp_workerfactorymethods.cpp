#include "vtcp_workerfactorymethods.h"
#include "vtcp_peerworker.h"

namespace VeinTcp
{

std::function<TcpPeerWorkerInterfacePtr(TcpPeer *)> TcpWorkerFactoryMethods::m_createFunctionPeer = [](TcpPeer *peer) {
    return std::make_unique<TcpPeerWorker>(peer, TcpPeerWorker::secret());
};

std::function<TcpPeerWorkerInterfacePtr(TcpPeer *, qintptr)> TcpWorkerFactoryMethods::m_createFunctionPeerDescriptor = [](TcpPeer *peer, qintptr socketDescriptor) {
    return std::make_unique<TcpPeerWorker>(peer, socketDescriptor, TcpPeerWorker::secret());
};

TcpPeerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpPeerWorker(TcpPeer *peer)
{
    return m_createFunctionPeer(peer);
}

TcpPeerWorkerInterfacePtr TcpWorkerFactoryMethods::createTcpPeerWorker(TcpPeer *peer, qintptr socketDescriptor)
{
    return m_createFunctionPeerDescriptor(peer, socketDescriptor);
}

}
