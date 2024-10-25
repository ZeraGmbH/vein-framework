#ifndef TCPWORKERFACTORY_H
#define TCPWORKERFACTORY_H

#include "abstracttcpworkerfactory.h"

namespace VeinTcp
{
class TcpWorkerFactory : public AbstractTcpWorkerFactory
{
public:
    TcpPeerWorkerInterfacePtr createPeerWorkerForClient(TcpPeer *peer) override;
    TcpServerWorkerInterfacePtr createWorkerForServer(TcpServer *server) override;
};
}

#endif // TCPWORKERFACTORY_H
