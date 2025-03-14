#include "test_core_stack_client.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_core_stack_client)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

void test_core_stack_client::subsriptionWithThrowAwayObjects()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    QSignalSpy spy(&clientStack, &VfCoreStackClient::sigSubscribed);
    bool clientConnected = false;
    // check event loop fired: connect after connect
    QObject::connect(&clientStack, &VfCoreStackClient::sigConnnectionEstablished, this, [&]() {
        clientConnected = true;
        clientStack.subscribeEntity(systemEntityId);
    });

    spy.wait(1000);
    QVERIFY(clientConnected);
    QCOMPARE(spy.count(), 1);

}
