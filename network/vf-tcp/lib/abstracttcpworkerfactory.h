#ifndef ABSTRACTTCPWORKERFACTORY_H
#define ABSTRACTTCPWORKERFACTORY_H

#include "vtcp_serverworkerinterface.h"
#include <memory>

namespace VeinTcp
{
class TcpPeer;
class TcpServer;


class AbstractTcpWorkerFactory
{
public:
    virtual TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) = 0;
    virtual TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) = 0;
};

typedef std::shared_ptr<AbstractTcpWorkerFactory> AbstractTcpWorkerFactoryPtr;
}

#endif // ABSTRACTTCPWORKERFACTORY_H
