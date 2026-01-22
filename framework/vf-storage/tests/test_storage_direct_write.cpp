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

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);
    future->getValueForWrite() = QString("future");

    QVERIFY(!writableDb->hasStoredValue(testEntityId, testComponentName));
    QCOMPARE(writableDb->findComponent(testEntityId, testComponentName), nullptr);
    QCOMPARE(writableDb->getStoredValue(testEntityId, testComponentName), QVariant());
    QVERIFY(!writableDb->getComponentList(testEntityId).contains(testComponentName));

    QVERIFY(checkAllFutureGetters(writableDb, QString("future")));
    QVERIFY(writableDb->getComponentListWithFutures(testEntityId).contains(testComponentName));
}

void test_storage_direct_write::addFuturesChangeValueVeinAdd()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    TimeMachineObject::feedEventLoop();

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);
    future->getValueForWrite() = QString("future");
    server->addComponent(testEntityId, testComponentName, QString("vein"), true);

    QVERIFY(writableDb->hasStoredValue(testEntityId, testComponentName));
    QCOMPARE(writableDb->findComponent(testEntityId, testComponentName)->getValue(), "vein");
    QCOMPARE(writableDb->getStoredValue(testEntityId, testComponentName), "vein");
    QVERIFY(writableDb->getComponentList(testEntityId).contains(testComponentName));

    QVERIFY(checkAllFutureGetters(writableDb, QString("vein")));
    QVERIFY(writableDb->getComponentListWithFutures(testEntityId).contains(testComponentName));
}

void test_storage_direct_write::addFutureDumpIntrospection()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);
    future->getValueForWrite() = QString("future");

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

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);
    future->getValueForWrite() = QString("future");
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

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);
    future->getValueForWrite() = QString("future");

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

    VeinStorage::AbstractStorageWritable* storage = serverNet.getStorageWritable();
    AbstractDatabaseDirectWrite *writableDb = storage->getDbWritable();
    StorageComponentPtr future = writableDb->getFutureComponentForWrite(testEntityId, testComponentName);
    future->getValueForWrite() = QString("future");
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

bool test_storage_direct_write::checkAllFutureGetters(AbstractDatabaseDirectWrite *writableDb, const QVariant &expectedValue)
{
    bool ok = true;
    if (writableDb->getFutureComponent(testEntityId, testComponentName)->getValue() != expectedValue) {
        qWarning("getFutureComponent()->getValue() unequal");
        ok = false;
    }
    if (writableDb->getFutureComponent(testEntityId, testComponentName)->getValueForWrite() != expectedValue) {
        qWarning("getFutureComponent()->getValueForWrite()() unequal");
        ok = false;
    }
    if (writableDb->getFutureComponentForWrite(testEntityId, testComponentName)->getValue() != expectedValue) {
        qWarning("getEntityComponentForWrite()->getValue() unequal");
        ok = false;
    }
    if (writableDb->getFutureComponentForWrite(testEntityId, testComponentName)->getValueForWrite() != expectedValue) {
        qWarning("getFutureComponentForWrite()->getValueForWrite() unequal");
        ok = false;
    }
    return ok;
}

bool test_storage_direct_write::checkSameIdentities(AbstractDatabaseDirectWrite *writableDb, const StorageComponentPtr component)
{
    bool ok = true;
    if (component != writableDb->getFutureComponent(testEntityId, testComponentName)) {
        qWarning("getFutureComponent()");
        ok = false;
    }
    if (component != writableDb->getFutureComponentForWrite(testEntityId, testComponentName)) {
        qWarning("getFutureComponentForWrite()");
        ok = false;
    }
    if (component != writableDb->findComponent(testEntityId, testComponentName)) {
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
