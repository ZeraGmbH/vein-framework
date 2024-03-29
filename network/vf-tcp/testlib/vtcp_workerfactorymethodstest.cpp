#include "vtcp_workerfactorymethodstest.h"
#include "vtcp_peerworkermock.h"
#include "vtcp_serverworkermock.h"
#include <functional>

namespace VeinTcp
{

void TcpWorkerFactoryMethodsTest::enableRealNetwork()
{
    m_createFunctionPeer = getCreateFunctionPeer();
    m_createFunctionServer = getCreateFunctionServer();
}

void TcpWorkerFactoryMethodsTest::enableMockNetwork()
{
    TcpServerWorkerMock::cleanupServerMocks();
    m_createFunctionPeer =
        [](TcpPeer *peer) {
            return std::make_unique<TcpPeerWorkerMock>(peer, TcpPeerWorkerMock::secret());
        };
    m_createFunctionServer =
        [](TcpServer *server) {
            return std::make_unique<TcpServerWorkerMock>(server, TcpServerWorkerMock::secret());
        };
}

}
