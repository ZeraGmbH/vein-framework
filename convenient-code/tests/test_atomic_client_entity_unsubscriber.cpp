#include "test_atomic_client_entity_unsubscriber.h"
#include "vfatomiccliententityunsubscriber.h"
#include "vfatomiccliententitysubscriber.h"
#include "vfcommandeventhandlersystem.h"
#include "ve_eventhandler.h"
#include "vfcorestackclient.h"
#include "vftestserverstack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_atomic_client_entity_unsubscriber)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

void test_atomic_client_entity_unsubscriber::unsubscribeOnNotSubscribed()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfAtomicClientEntityUnsubscriberPtr entityToUnsubscribe = VfAtomicClientEntityUnsubscriber::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityToUnsubscribe);
    QSignalSpy spy(entityToUnsubscribe.get(), &VfAtomicClientEntityUnsubscriber::sigUnsubscribed);
    entityToUnsubscribe->sendUnsubscription();
    feedEventLoop();

    // No response
    QCOMPARE(spy.count(), 0);
}

void test_atomic_client_entity_unsubscriber::subscribeUnsubscribe()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfAtomicClientEntitySubscriberPtr entityToSubscribe = VfAtomicClientEntitySubscriber::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityToSubscribe);
    QSignalSpy spySubscribe(entityToSubscribe.get(), &VfAtomicClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    feedEventLoop();
    QCOMPARE(spySubscribe.count(), 1);
    QCOMPARE(spySubscribe[0][0].toBool(), true);

    VfAtomicClientEntityUnsubscriberPtr entityToUnsubscribe = VfAtomicClientEntityUnsubscriber::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityToUnsubscribe);
    QSignalSpy spy(entityToUnsubscribe.get(), &VfAtomicClientEntityUnsubscriber::sigUnsubscribed);
    entityToUnsubscribe->sendUnsubscription();
    feedEventLoop();

    // No response
    QCOMPARE(spy.count(), 0);
}

void test_atomic_client_entity_unsubscriber::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
