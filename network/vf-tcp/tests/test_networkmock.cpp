#include "test_networkmock.h"
#include "vfcommandeventhandlersystem.h"
#include "vfsentityinsubscription.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_networkmock)

static constexpr int systemEntityId = 0;
static constexpr int serverPort = 4242;

struct TestServerStack
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    TestServerStack() :
        server(&eventHandler)
    {
        eventHandler.addSubsystem(&netSystem);
        eventHandler.addSubsystem(&tcpSystem);
        tcpSystem.startServer(serverPort, false);
        while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
    }
};

struct TestClientStack
{
    VeinEvent::EventHandler eventHandler;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    TestClientStack()
    {
        netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
        eventHandler.addSubsystem(&netSystem);
        eventHandler.addSubsystem(&tcpSystem);
    }
};

void test_networkmock::receiveIntrospectionOverRealNetwork()
{
    TestServerStack serverStack;

    TestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    VfsEntityInSubscriptionPtr entityToSubscribe = VfsEntityInSubscription::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);

    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    QSignalSpy spy(entityToSubscribe.get(), &VfsEntityInSubscription::sigSubscribed);
    bool clientConnected = false;
    QObject::connect(&clientStack.tcpSystem, &VeinNet::TcpSystem::sigConnnectionEstablished, [&]() {
        clientConnected = true;
        entityToSubscribe->sendSubscrption();
    });
    spy.wait(1000);
    QVERIFY(clientConnected);
    QCOMPARE(spy.count(), 1);
}

void test_networkmock::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
