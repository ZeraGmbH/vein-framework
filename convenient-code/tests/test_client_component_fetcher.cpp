#include "test_client_component_fetcher.h"
#include "vf_client_component_fetcher.h"
#include "veintestserver.h"
#include "vf_core_stack_client.h"
#include "vf_test_server_stack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_component_fetcher)

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet() :
        server(&eventHandler)
    {
        eventHandler.addSubsystem(&cmdEventHandlerSystem);
    }
};

void test_client_component_fetcher::errorSignalFromUnsubscribedEntityInvalidComponentNoNet()
{
    ServerNoNet server;
    feedEventLoop();
    QList<int> entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("foo", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy spy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_client_component_fetcher::getFromUnsubscribedEntityValidComponentNoNet()
{
    ServerNoNet server;
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy spy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_client_component_fetcher::noGetFromUnsubscribedEntityValidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 0);
}

void test_client_component_fetcher::getFromSubscribedEntityValidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 1);
    QList<QVariant> arguments = fetcherSpy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_client_component_fetcher::getFromSubscribedEntityInvalidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("foo", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 1);
    QList<QVariant> arguments = fetcherSpy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_client_component_fetcher::getTwoDifferentComponent()
{
    // just server / no subscription
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server(&eventHandler);
    server.simulAllModulesLoaded("fooPath", QStringList() << "fooPath");
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher1 = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher1);
    QSignalSpy fetcherSpy1(fetcher1.get(), &VfClientComponentFetcher::sigGetFinish);
    
    VfClientComponentFetcherPtr fetcher2 = VfClientComponentFetcher::create("Session", entityItem);
    entityItem->addItem(fetcher2);
    QSignalSpy fetcherSpy2(fetcher2.get(), &VfClientComponentFetcher::sigGetFinish);

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

void test_client_component_fetcher::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
