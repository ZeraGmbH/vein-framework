#include "vtcp_workerfactorymethodstest.h"
#include "mocknetpeerworker.h"
#include "mocknetserverworker.h"
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
    MockNetServerWorker::cleanupServerMocks();
    m_createFunctionPeer =
        [](TcpPeer *peer) {
            return std::make_unique<MockNetPeerWorker>(peer, MockNetPeerWorker::secret());
        };
    m_createFunctionServer =
        [](TcpServer *server) {
            return std::make_unique<MockNetServerWorker>(server, MockNetServerWorker::secret());
        };
}

}
