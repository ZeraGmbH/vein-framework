#ifndef ABSTRACTTCPWORKERFACTORY_H
#define ABSTRACTTCPWORKERFACTORY_H

#include "vtcp_peer.h"
#include "vtcp_server.h"
#include "vtcp_serverworkerinterface.h"

namespace VeinTcp
{
class AbstractTcpWorkerFactory
{
public:
    virtual TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) = 0;
    virtual TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) = 0;
};
}

#endif // ABSTRACTTCPWORKERFACTORY_H
