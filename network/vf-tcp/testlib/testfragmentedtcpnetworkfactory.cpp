#include "testfragmentedtcpnetworkfactory.h"
#include "testfragmentednetpeerworker.h"
#include "testfragmentednetserverworker.h"

namespace VeinTcp
{

AbstractTcpNetworkFactoryPtr TestFragmentedTcpNetworkFactory::create(qint64 writeSizeServer, qint64 writeSizeClient)
{
    return std::make_shared<TestFragmentedTcpNetworkFactory>(writeSizeServer, writeSizeClient);
}

TestFragmentedTcpNetworkFactory::TestFragmentedTcpNetworkFactory(qint64 readBufferSizeServer, qint64 readBufferSizeClient) :
    m_writeSizeServer(readBufferSizeServer),
    m_writeSizeClient(readBufferSizeClient)
{
}

TcpPeerWorkerInterfacePtr TestFragmentedTcpNetworkFactory::createTcpPeerWorker(TcpPeer *peer)
{
    return std::make_unique<TestFragmentedNetPeerWorker>(peer, m_writeSizeClient);
}

TcpServerWorkerInterfacePtr TestFragmentedTcpNetworkFactory::createTcpServerWorker(TcpServer *server)
{
    return std::make_unique<TestFragmentedNetServerWorker>(server, m_writeSizeServer);
}

}
