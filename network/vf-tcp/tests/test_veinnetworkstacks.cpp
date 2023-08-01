#include "test_veinnetworkstacks.h"
#include "vf_cmd_event_handler_system.h"
#include "vf_client_entity_subscriber.h"
#include "ve_eventhandler.h"
#include "vn_tcpsystem.h"
#include "vtcp_workerfactorymethodstest.h"
#include "vf_test_server_stack.h"
#include "vf_test_client_stack.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_veinnetworkstacks)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

void test_veinnetworkstacks::receiveIntrospection()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    VfClientEntitySubscriberPtr entityToSubscribe = VfClientEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);

    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    QSignalSpy spy(entityToSubscribe.get(), &VfClientEntitySubscriber::sigSubscribed);
    bool clientConnected = false;
    // check event loop fired: connect after connect
    QObject::connect(&clientStack.tcpSystem, &VeinNet::TcpSystem::sigConnnectionEstablished, [&]() {
        clientConnected = true;
        entityToSubscribe->sendSubscription();
    });
    spy.wait(1000);
    QVERIFY(clientConnected);
    QCOMPARE(spy.count(), 1);
}

void test_veinnetworkstacks::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
