#ifndef TCPWORKERFACTORY_H
#define TCPWORKERFACTORY_H

#include "abstracttcpworkerfactory.h"

namespace VeinTcp
{
class TcpWorkerFactory : public AbstractTcpWorkerFactory
{
public:
    TcpPeerWorkerInterfacePtr createPeerWorkerForClient(TcpPeer *peer, NETWORK network) override;
    TcpServerWorkerInterfacePtr createWorkerForServer(TcpServer *server, NETWORK network) override;
};
}

#endif // TCPWORKERFACTORY_H
