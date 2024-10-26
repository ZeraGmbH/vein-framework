#ifndef TCPWORKERFACTORY_H
#define TCPWORKERFACTORY_H

#include "abstracttcpworkerfactory.h"

namespace VeinTcp
{
class TcpWorkerFactory : public AbstractTcpWorkerFactory
{
public:
    static AbstractTcpWorkerFactoryPtr create();
    TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) override;
    TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) override;
};
}

#endif // TCPWORKERFACTORY_H
