#ifndef TCPWORKERFACTORYMETHODS_H
#define TCPWORKERFACTORYMETHODS_H

#include "vtcp_peer.h"
#include "vtcp_server.h"
#include "vtcp_peerworkerinterface.h"
#include "vtcp_serverworkerinterface.h"
#include <functional>

namespace VeinTcp
{

class TcpNetworkFactoryMethods
{
public:
    static TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer);
    static TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server);
protected:
    static std::function<TcpPeerWorkerInterfacePtr(TcpPeer *)> getCreateFunctionPeer();
    static std::function<TcpServerWorkerInterfacePtr(TcpServer *)> getCreateFunctionServer();

    static std::function<TcpPeerWorkerInterfacePtr(TcpPeer *)> m_createFunctionPeer;
    static std::function<TcpServerWorkerInterfacePtr(TcpServer *)> m_createFunctionServer;
};

}
#endif // TCPWORKERFACTORYMETHODS_H
