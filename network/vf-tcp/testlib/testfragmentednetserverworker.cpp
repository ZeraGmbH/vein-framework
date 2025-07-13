#include "testfragmentednetserverworker.h"
#include "testfragmentednetpeerworker.h"

namespace VeinTcp
{

TestFragmentedNetServerWorker::TestFragmentedNetServerWorker(TcpServer *server, qint64 readBufferSize) :
    TcpServerWorker(server),
    m_readBufferSize(readBufferSize)
{
}

TcpPeerWorkerInterfacePtr TestFragmentedNetServerWorker::createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor)
{
    return std::make_unique<TestFragmentedNetPeerWorker>(peer, socketDescriptor, m_readBufferSize);
}

}
