#ifndef TESTFRAGMENTEDTCPNETWORKFACTORY_H
#define TESTFRAGMENTEDTCPNETWORKFACTORY_H

#include <abstracttcpnetworkfactory.h>

namespace VeinTcp
{
class TestFragmentedTcpNetworkFactory : public AbstractTcpNetworkFactory
{
public:
    static AbstractTcpNetworkFactoryPtr create(qint64 writeSizeServer, qint64 writeSizeClient);
    TestFragmentedTcpNetworkFactory(qint64 readBufferSizeServer, qint64 readBufferSizeClient);
    TcpPeerWorkerInterfacePtr createTcpPeerWorker(TcpPeer *peer) override;
    TcpServerWorkerInterfacePtr createTcpServerWorker(TcpServer *server) override;
private:
    qint64 m_writeSizeServer;
    qint64 m_writeSizeClient;
};
}

#endif // TESTFRAGMENTEDTCPNETWORKFACTORY_H
