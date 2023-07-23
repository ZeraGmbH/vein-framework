#include "vtcp_workerfactorymethodstest.h"
#include "vtcp_peerworkermock.h"
#include "vtcp_serverworkermock.h"
#include <functional>

namespace VeinTcp
{

void TcpWorkerFactoryMethodsTest::enableProduction()
{
    m_createFunctionPeer = getCreateFunctionPeer();
    m_createFunctionPeerWithDescriptor = getCreateFunctionPeerWithDescriptor();
    m_createFunctionServer = getCreateFunctionServer();
}

void TcpWorkerFactoryMethodsTest::enableMock()
{
    m_createFunctionPeer =
        [](TcpPeer *peer) { return std::make_unique<TcpPeerWorkerMock>(peer, TcpPeerWorkerMock::secret()); };
    m_createFunctionPeerWithDescriptor =
        [](TcpPeer *peer, qintptr socketDescriptor) { return std::make_unique<TcpPeerWorkerMock>(peer, socketDescriptor, TcpPeerWorkerMock::secret()); };
    m_createFunctionServer =
        [](TcpServer *server) { return std::make_unique<TcpServerWorkerMock>(server, TcpServerWorkerMock::secret()); };
}

}
