#include "test_atomic_client_component_fetcher.h"
#include "vfatomicclientcomponentfetcher.h"
#include "veintestserver.h"
#include "vftestclientstack.h"
#include "vftestserverstack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_atomic_client_component_fetcher)

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet() :
        server(&eventHandler)
    {
        eventHandler.addSubsystem(&cmdEventHandlerSystem);
    }
};

void test_atomic_client_component_fetcher::errorSignalFromUnsubscribedEntityInvalidComponentNoNet()
{
    ServerNoNet server;
    feedEventLoop();
    QList<int> entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentFetcherPtr fetcher = VfAtomicClientComponentFetcher::create("foo", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy spy(fetcher.get(), &VfAtomicClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_atomic_client_component_fetcher::getFromUnsubscribedEntityValidComponentNoNet()
{
    ServerNoNet server;
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentFetcherPtr fetcher = VfAtomicClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy spy(fetcher.get(), &VfAtomicClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_atomic_client_component_fetcher::noGetFromUnsubscribedEntityValidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentFetcherPtr fetcher = VfAtomicClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfAtomicClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 0);
}

void test_atomic_client_component_fetcher::getFromSubscribedEntityValidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    clientStack.subscribeEntityId(systemEntityId, &cmdEventHandlerSystem);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentFetcherPtr fetcher = VfAtomicClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfAtomicClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 1);
    QList<QVariant> arguments = fetcherSpy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_atomic_client_component_fetcher::getFromSubscribedEntityInvalidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    clientStack.subscribeEntityId(systemEntityId, &cmdEventHandlerSystem);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentFetcherPtr fetcher = VfAtomicClientComponentFetcher::create("foo", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfAtomicClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 1);
    QList<QVariant> arguments = fetcherSpy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_atomic_client_component_fetcher::getTwoDifferentComponent()
{
    // just server / no subscription
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server(&eventHandler);
    server.simulAllModulesLoaded("fooPath", QStringList() << "fooPath");
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentFetcherPtr fetcher1 = VfAtomicClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher1);
    QSignalSpy fetcherSpy1(fetcher1.get(), &VfAtomicClientComponentFetcher::sigGetFinish);
    
    VfAtomicClientComponentFetcherPtr fetcher2 = VfAtomicClientComponentFetcher::create("Session", entityItem);
    entityItem->addItem(fetcher2);
    QSignalSpy fetcherSpy2(fetcher2.get(), &VfAtomicClientComponentFetcher::sigGetFinish);

    fetcher1->startGetComponent();
    fetcher2->startGetComponent();
    feedEventLoop();

    QCOMPARE(fetcherSpy1.count(), 1);
    QList<QVariant> arguments1 = fetcherSpy1[0];
    QCOMPARE(arguments1.at(0).toBool(), true);
    QCOMPARE(arguments1.at(1), QVariant("_System"));

    QCOMPARE(fetcherSpy2.count(), 1);
    QList<QVariant> arguments2 = fetcherSpy2[0];
    QCOMPARE(arguments2.at(0).toBool(), true);
    QCOMPARE(arguments2.at(1), QVariant("fooPath"));
}

void test_atomic_client_component_fetcher::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
