#include "test_client_component_fetcher.h"
#include "vf_client_component_fetcher.h"
#include "testveinserver.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpworkerfactory.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_component_fetcher)

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    VeinEvent::EventHandler eventHandler;
    TestVeinServer server;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet()
    {
        server.appendEventSystem(&cmdEventHandlerSystem);
    }
};

void test_client_component_fetcher::errorSignalFromUnsubscribedEntityInvalidComponentNoNet()
{
    ServerNoNet server;
    TimeMachineObject::feedEventLoop();
    QList<int> entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("foo", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy spy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_client_component_fetcher::getFromUnsubscribedEntityValidComponentNoNet()
{
    ServerNoNet server;
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy spy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_client_component_fetcher::noGetFromUnsubscribedEntityValidComponentNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack;
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    clientStack.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 0);
}

void test_client_component_fetcher::getFromSubscribedEntityValidComponentNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpWorkerFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    clientStack.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 1);
    QList<QVariant> arguments = fetcherSpy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_client_component_fetcher::getFromSubscribedEntityInvalidComponentNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpWorkerFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    clientStack.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create("foo", entityItem);
    entityItem->addItem(fetcher);
    QSignalSpy fetcherSpy(fetcher.get(), &VfClientComponentFetcher::sigGetFinish);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(fetcherSpy.count(), 1);
    QList<QVariant> arguments = fetcherSpy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_client_component_fetcher::getTwoDifferentComponent()
{
    // just server / no subscription
    TestVeinServer server;
    server.simulAllModulesLoaded("fooPath", QStringList() << "fooPath");
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    server.appendEventSystem(&cmdEventHandlerSystem);
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfClientComponentFetcherPtr fetcher1 = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(fetcher1);
    QSignalSpy fetcherSpy1(fetcher1.get(), &VfClientComponentFetcher::sigGetFinish);
    
    VfClientComponentFetcherPtr fetcher2 = VfClientComponentFetcher::create("Session", entityItem);
    entityItem->addItem(fetcher2);
    QSignalSpy fetcherSpy2(fetcher2.get(), &VfClientComponentFetcher::sigGetFinish);

    fetcher1->startGetComponent();
    fetcher2->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(fetcherSpy1.count(), 1);
    QList<QVariant> arguments1 = fetcherSpy1[0];
    QCOMPARE(arguments1.at(0).toBool(), true);
    QCOMPARE(arguments1.at(1), QVariant("_System"));

    QCOMPARE(fetcherSpy2.count(), 1);
    QList<QVariant> arguments2 = fetcherSpy2[0];
    QCOMPARE(arguments2.at(0).toBool(), true);
    QCOMPARE(arguments2.at(1), QVariant("fooPath"));
}
