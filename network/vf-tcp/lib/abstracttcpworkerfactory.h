#ifndef ABSTRACTTCPWORKERFACTORY_H
#define ABSTRACTTCPWORKERFACTORY_H

#include "vtcp_serverworkerinterface.h"

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
}

#endif // ABSTRACTTCPWORKERFACTORY_H
