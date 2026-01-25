#include "test_storage_direct_write.h"
#include "mocktcpnetworkfactory.h"
#include "vf_client_component_fetcher.h"
#include "vf_entity_component_event_item.h"
#include <testloghelpers.h>
#include <timemachineobject.h>
#include <QTest>

QTEST_MAIN(test_storage_direct_write)

static constexpr int serverPort = 4242;
static constexpr int testEntityId = 37;
static const char* entityName = "TestEntity";
static const char* testComponentName = "TestComponent";

void test_storage_direct_write::addFuturesChangeValue()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    TimeMachineObject::feedEventLoop();

    VeinStorage::AbstractEventSystem* storage = serverNet.getStorage();
    AbstractDatabase *db = storage->getDb();
    AbstractComponentPtr future = db->getFutureComponent(testEntityId, testComponentName);
    future->setValue(QString("future"));

    QVERIFY(!db->hasStoredValue(testEntityId, testComponentName));
    QCOMPARE(db->findComponent(testEntityId, testComponentName), nullptr);
    QCOMPARE(db->getStoredValue(testEntityId, testComponentName), QVariant());
    QVERIFY(!db->getComponentList(testEntityId).contains(testComponentName));
    QVERIFY(db->getComponentListWithFutures(testEntityId).contains(testComponentName));
}

void test_storage_direct_write::addFuturesChangeValueVeinAdd()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    TimeMachineObject::feedEventLoop();

    VeinStorage::AbstractEventSystem* storage = serverNet.getStorage();
    AbstractDatabase *db = storage->getDb();
    AbstractComponentPtr future = db->getFutureComponent(testEntityId, testComponentName);
    future->setValue(QString("future"));
    server->addComponent(testEntityId, testComponentName, QString("vein"), true);

    QVERIFY(db->hasStoredValue(testEntityId, testComponentName));
    QCOMPARE(db->findComponent(testEntityId, testComponentName)->getValue(), "vein");
    QCOMPARE(db->getStoredValue(testEntityId, testComponentName), "vein");
    QVERIFY(db->getComponentList(testEntityId).contains(testComponentName));
    QVERIFY(db->getComponentListWithFutures(testEntityId).contains(testComponentName));
}

void test_storage_direct_write::addFutureDumpIntrospection()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);

    VeinStorage::AbstractEventSystem* storage = serverNet.getStorage();
    AbstractDatabase *db = storage->getDb();
    AbstractComponentPtr future = db->getFutureComponent(testEntityId, testComponentName);
    future->setValue(QString("future"));

    VfCoreStackClient clientNet(VeinTcp::MockTcpNetworkFactory::create());
    clientNet.connectToServer("127.0.0.1", serverPort);
    QJsonObject jsonEvents;
    setupSpy(serverNet, clientNet, jsonEvents);

    clientNet.subscribeEntity(testEntityId);
    TimeMachineObject::feedEventLoop();

    QByteArray expected = TestLogHelpers::loadFile(":/vein-storage-events/add-future-no-vein-introspection.json");
    QByteArray dumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(expected, dumped));
}

void test_storage_direct_write::addFutureAddVeinDumpIntrospection()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);

    VeinStorage::AbstractEventSystem* storage = serverNet.getStorage();
    AbstractDatabase *db = storage->getDb();
    AbstractComponentPtr future = db->getFutureComponent(testEntityId, testComponentName);
    future->setValue(QString("future"));
    server->addComponent(testEntityId, testComponentName, QString("vein"), true);

    VfCoreStackClient clientNet(VeinTcp::MockTcpNetworkFactory::create());
    clientNet.connectToServer("127.0.0.1", serverPort);
    QJsonObject jsonEvents;
    setupSpy(serverNet, clientNet, jsonEvents);

    clientNet.subscribeEntity(testEntityId);
    TimeMachineObject::feedEventLoop();

    QByteArray expected = TestLogHelpers::loadFile(":/vein-storage-events/add-future-add-vein-introspection.json");
    QByteArray dumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(expected, dumped));
}

void test_storage_direct_write::addFutureFetch()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);

    VeinStorage::AbstractEventSystem* storage = serverNet.getStorage();
    AbstractDatabase *db = storage->getDb();
    AbstractComponentPtr future = db->getFutureComponent(testEntityId, testComponentName);
    future->setValue(QString("future"));

    VfCoreStackClient clientNet(VeinTcp::MockTcpNetworkFactory::create());
    clientNet.connectToServer("127.0.0.1", serverPort);
    clientNet.subscribeEntity(testEntityId);
    TimeMachineObject::feedEventLoop();

    QJsonObject jsonEvents;
    setupSpy(serverNet, clientNet, jsonEvents);

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testEntityId);
    clientNet.addItem(entityItem);
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create(testComponentName, entityItem);
    entityItem->addItem(fetcher);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QByteArray expected = TestLogHelpers::loadFile(":/vein-storage-events/add-future-no-vein-fetch.json");
    QByteArray dumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(expected, dumped));
}

void test_storage_direct_write::addFutureAddVeinFetch()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);

    VeinStorage::AbstractEventSystem* storage = serverNet.getStorage();
    AbstractDatabase *db = storage->getDb();
    AbstractComponentPtr future = db->getFutureComponent(testEntityId, testComponentName);
    future->setValue(QString("future"));
    server->addComponent(testEntityId, testComponentName, QString("vein"), true);

    VfCoreStackClient clientNet(VeinTcp::MockTcpNetworkFactory::create());
    clientNet.connectToServer("127.0.0.1", serverPort);
    clientNet.subscribeEntity(testEntityId);
    TimeMachineObject::feedEventLoop();

    QJsonObject jsonEvents;
    setupSpy(serverNet, clientNet, jsonEvents);

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testEntityId);
    clientNet.addItem(entityItem);
    VfClientComponentFetcherPtr fetcher = VfClientComponentFetcher::create(testComponentName, entityItem);
    entityItem->addItem(fetcher);
    fetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QByteArray expected = TestLogHelpers::loadFile(":/vein-storage-events/add-future-add-vein-fetch.json");
    QByteArray dumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(expected, dumped));
}

bool test_storage_direct_write::checkSameIdentities(AbstractDatabase *db, const AbstractComponentPtr component)
{
    bool ok = true;
    if (component != db->getFutureComponent(testEntityId, testComponentName)) {
        qWarning("getFutureComponent()");
        ok = false;
    }
    if (component != db->findComponent(testEntityId, testComponentName)) {
        qWarning("findComponent()");
        ok = false;
    }
    return ok;
}

void test_storage_direct_write::setupSpy(TestVeinServerWithMockNet &serverNet, VfCoreStackClient &clientNet, QJsonObject &jsonEvents)
{
    m_serverCmdEventSpyTop = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "server-enter");
    serverNet.getServer()->prependEventSystem(m_serverCmdEventSpyTop.get());

    m_clientCmdEventSpyTop = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "client-enter");
    clientNet.prependEventSystem(m_clientCmdEventSpyTop.get());
}
