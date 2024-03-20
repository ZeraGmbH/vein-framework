#ifndef ABSTRACTTCPWORKERFACTORY_H
#define ABSTRACTTCPWORKERFACTORY_H

#include "vtcp_peer.h"
#include "vtcp_server.h"
#include "vtcp_serverworkerinterface.h"

namespace VeinTcp
{
enum NETWORK
{
    SERVICE_NET,
    VEIN_NET
};

class AbstractTcpWorkerFactory
{
public:
    virtual TcpPeerWorkerInterfacePtr createPeerWorkerForClient(TcpPeer *peer, NETWORK network) = 0;
    virtual TcpServerWorkerInterfacePtr createWorkerForServer(TcpServer *server, NETWORK network) = 0;
};
}

#endif // ABSTRACTTCPWORKERFACTORY_H
