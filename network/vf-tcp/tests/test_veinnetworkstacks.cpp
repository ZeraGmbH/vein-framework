#include "test_veinnetworkstacks.h"
#include "vfcommandeventhandlersystem.h"
#include "vfsimpleentitysubscriber.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"
#include "vtcp_workerfactorymethodstest.h"
#include "testserverstack.h"
#include "testclientstack.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_veinnetworkstacks)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

void test_veinnetworkstacks::receiveIntrospection()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestServerStack serverStack(serverPort);

    TestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    VfSimpleEntitySubscriberPtr entityToSubscribe = VfSimpleEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);

    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    QSignalSpy spy(entityToSubscribe.get(), &VfSimpleEntitySubscriber::sigSubscribed);
    bool clientConnected = false;
    // check event loop fired: connect after connect
    QObject::connect(&clientStack.tcpSystem, &VeinNet::TcpSystem::sigConnnectionEstablished, [&]() {
        clientConnected = true;
        entityToSubscribe->sendSubscrption();
    });
    spy.wait(1000);
    QVERIFY(clientConnected);
    QCOMPARE(spy.count(), 1);
}

void test_veinnetworkstacks::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
