#ifndef TCPWORKERFACTORY_H
#define TCPWORKERFACTORY_H

#include "abstracttcpnetworkfactory.h"

namespace VeinTcp
{
class TcpNetworkFactory : public AbstractTcpNetworkFactory
{
public:
    static AbstractTcpNetworkFactoryPtr create();
    TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) override;
    TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) override;
};
}

#endif // TCPWORKERFACTORY_H
