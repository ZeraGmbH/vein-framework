#ifndef ABSTRACTTCPNETWORKFACTORY_H
#define ABSTRACTTCPNETWORKFACTORY_H

#include "vtcp_serverworkerinterface.h"
#include <memory>

namespace VeinTcp
{
class TcpPeer;
class TcpServer;


class AbstractTcpNetworkFactory
{
public:
    virtual TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) = 0;
    virtual TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) = 0;
};

typedef std::shared_ptr<AbstractTcpNetworkFactory> AbstractTcpNetworkFactoryPtr;
}

#endif // ABSTRACTTCPNETWORKFACTORY_H
