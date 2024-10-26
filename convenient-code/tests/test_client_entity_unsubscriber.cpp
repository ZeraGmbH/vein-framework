#include "test_client_entity_unsubscriber.h"
#include "vf_client_entity_unsubscriber.h"
#include "vf_client_entity_subscriber.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpworkerfactory.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_entity_unsubscriber)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

void test_client_entity_unsubscriber::unsubscribeOnNotSubscribed()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpWorkerFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfClientEntityUnsubscriberPtr entityToUnsubscribe = VfClientEntityUnsubscriber::create(systemEntityId);
    clientStack.addItem(entityToUnsubscribe);
    QSignalSpy spy(entityToUnsubscribe.get(), &VfClientEntityUnsubscriber::sigUnsubscribed);
    entityToUnsubscribe->sendUnsubscription();
    TimeMachineObject::feedEventLoop();

    // No response
    QCOMPARE(spy.count(), 0);
}

void test_client_entity_unsubscriber::subscribeUnsubscribe()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpWorkerFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    clientStack.addItem(entityToSubscribe);
    QSignalSpy spySubscribe(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spySubscribe.count(), 1);
    QCOMPARE(spySubscribe[0][0].toBool(), true);

    VfClientEntityUnsubscriberPtr entityToUnsubscribe = VfClientEntityUnsubscriber::create(systemEntityId);
    clientStack.addItem(entityToUnsubscribe);
    QSignalSpy spy(entityToUnsubscribe.get(), &VfClientEntityUnsubscriber::sigUnsubscribed);
    entityToUnsubscribe->sendUnsubscription();
    TimeMachineObject::feedEventLoop();

    // No response
    QCOMPARE(spy.count(), 0);
}
