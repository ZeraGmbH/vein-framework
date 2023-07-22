#ifndef TCPWORKERFACTORYMETHODS_H
#define TCPWORKERFACTORYMETHODS_H

#include "vtcp_peer.h"
#include "vtcp_peerworkerinterface.h"
#include <functional>

namespace VeinTcp
{

class TcpWorkerFactoryMethods
{
public:
    static TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer);
    static TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer, qintptr socketDescriptor);
protected:
    static std::function<TcpPeerWorkerInterfacePtr(TcpPeer *)> m_createFunctionPeer;
    static std::function<TcpPeerWorkerInterfacePtr(TcpPeer *, qintptr)> m_createFunctionPeerDescriptor;
};

}
#endif // TCPWORKERFACTORYMETHODS_H
