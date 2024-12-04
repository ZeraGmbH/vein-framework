#include "test_subscribe.h"
#include "vf_client_entity_subscriber.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_subscribe)

static constexpr int systemEntityId = 0;
static constexpr int noneExistentEnitityId = 42;
static constexpr int serverPort = 4242;

void test_subscribe::intropectSendToSubscriberOnly()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack1(VeinTcp::MockTcpNetworkFactory::create());
    clientStack1.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();
    VfClientEntitySubscriberPtr entityToSubscribe1 = VfClientEntitySubscriber::create(systemEntityId);
    clientStack1.addItem(entityToSubscribe1);
    QSignalSpy spy1(entityToSubscribe1.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe1->sendSubscription();
    TimeMachineObject::feedEventLoop();

    VfCoreStackClient clientStack2(VeinTcp::MockTcpNetworkFactory::create());
    clientStack2.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();
    VfClientEntitySubscriberPtr entityToSubscribe2 = VfClientEntitySubscriber::create(systemEntityId);
    clientStack2.addItem(entityToSubscribe2);
    QSignalSpy spy2(entityToSubscribe2.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe2->sendSubscription();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);
}
