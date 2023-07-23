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

void test_networkmock::receiveIntrospectionOverRealNetwork()
{
    // server
    VeinEvent::EventHandler eventHandlerServer;
    VeinTestServer testServer(&eventHandlerServer);
    VeinNet::NetworkSystem netSystemServer;
    VeinNet::TcpSystem tcpSystemServer;
    eventHandlerServer.addSubsystem(&netSystemServer);
    eventHandlerServer.addSubsystem(&tcpSystemServer);
    tcpSystemServer.startServer(serverPort, false);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    // client
    VeinEvent::EventHandler eventHandlerClient;
    VeinNet::NetworkSystem netSystemClient;
    netSystemClient.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    VeinNet::TcpSystem tcpSystemClient;
    eventHandlerClient.addSubsystem(&netSystemClient);
    eventHandlerClient.addSubsystem(&tcpSystemClient);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandlerClient.addSubsystem(&cmdEventHandlerSystem);
    VfsEntityInSubscriptionPtr entityToSubscribe = VfsEntityInSubscription::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);

    tcpSystemClient.connectToServer("127.0.0.1", serverPort);
    QSignalSpy spy(entityToSubscribe.get(), &VfsEntityInSubscription::sigSubscribed);
    bool clientConnected = false;
    QObject::connect(&tcpSystemClient, &VeinNet::TcpSystem::sigConnnectionEstablished, [&]() {
        clientConnected = true;
        entityToSubscribe->sendSubscrption();
    });
    spy.wait(5000);
    QVERIFY(clientConnected);
    QCOMPARE(spy.count(), 1);
}

void test_networkmock::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
