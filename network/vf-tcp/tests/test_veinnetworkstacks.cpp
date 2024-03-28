#include "test_veinnetworkstacks.h"
#include "vf_cmd_event_handler_system.h"
#include "vf_client_entity_subscriber.h"
#include "vf_core_stack_client.h"
#include "vn_tcpsystem.h"
#include "vtcp_workerfactorymethodstest.h"
#include "testveinserverwithnet.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_veinnetworkstacks)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

void test_veinnetworkstacks::receiveIntrospection()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestVeinServerWithNet serverNet(serverPort);

    VfCoreStackClient clientStack;
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityToSubscribe);

    clientStack.connectToServer("127.0.0.1", serverPort);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    bool clientConnected = false;
    // check event loop fired: connect after connect
    QObject::connect(clientStack.getTcpSystem(), &VeinNet::TcpSystem::sigConnnectionEstablished, [&]() {
        clientConnected = true;
        entityToSubscribe->sendSubscription();
    });
    spy.wait(1000);
    QVERIFY(clientConnected);
    QCOMPARE(spy.count(), 1);
}
