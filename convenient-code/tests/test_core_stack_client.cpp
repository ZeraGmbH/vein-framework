#include "test_core_stack_client.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithnet.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_core_stack_client)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

void test_core_stack_client::subsriptionWithThrowAwayObjects()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestVeinServerWithNet serverNet(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);

    QSignalSpy spy(&clientStack, &VfCoreStackClient::sigSubscribed);
    bool clientConnected = false;
    // check event loop fired: connect after connect
    QObject::connect(&clientStack.tcpSystem, &VeinNet::TcpSystem::sigConnnectionEstablished, this, [&]() {
        clientConnected = true;
        clientStack.subscribeEntity(systemEntityId);
    });

    spy.wait(1000);
    QVERIFY(clientConnected);
    QCOMPARE(spy.count(), 1);

}
