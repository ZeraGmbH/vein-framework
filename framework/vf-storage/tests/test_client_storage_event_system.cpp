#include "test_client_storage_event_system.h"
#include "mocktcpnetworkfactory.h"
#include "vf_client_component_fetcher.h"
#include "vf_client_component_setter.h"
#include "vf_client_rpc_invoker.h"
#include "vf_entity_rpc_event_handler.h"
#include "testjsonspyeventsystem.h"
#include "testloghelpers.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_storage_event_system)

static constexpr int entityID1 = 10;

void test_client_storage_event_system::init()
{
    constexpr int serverPort = 4242;
    m_netServer = std::make_unique<TestVeinServerWithMockNet>(serverPort);
    m_netClient = std::make_unique<VfCoreStackClient>(VeinTcp::MockTcpNetworkFactory::create());
    m_clientStorageSystem = std::make_unique<VeinStorage::ClientStorageEventSystem>();
    m_netClient->appendEventSystem(m_clientStorageSystem.get());
    m_netClient->connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();
}

void test_client_storage_event_system::cleanup()
{
    m_clientStorageSystem = nullptr;
    m_netClient = nullptr;
    m_netServer = nullptr;
}

void test_client_storage_event_system::setupSpy(QJsonObject &jsonEvents)
{
    m_serverCmdEventSpyTop = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "server-enter");
    m_netServer->getServer()->prependEventSystem(m_serverCmdEventSpyTop.get());

    m_serverCmdEventSpyBottom = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "server-fallthrough");
    connect(m_netServer->getServer()->getEventHandler(), &VeinEvent::EventHandler::sigEventAccepted,
            m_serverCmdEventSpyBottom.get(), &TestJsonSpyEventSystem::onEventAccepted);
    m_netServer->getServer()->appendEventSystem(m_serverCmdEventSpyBottom.get());

    m_clientCmdEventSpyTop = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "client-enter");
    m_netClient->prependEventSystem(m_clientCmdEventSpyTop.get());

    m_clientCmdEventSpyBottom = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "client-fallthrough");
    connect(m_netClient->getEventHandler(), &VeinEvent::EventHandler::sigEventAccepted,
            m_clientCmdEventSpyBottom.get(), &TestJsonSpyEventSystem::onEventAccepted);
    m_netClient->appendEventSystem(m_clientCmdEventSpyBottom.get());
}

void test_client_storage_event_system::subscribeToNonExistingEntity()
{
    m_netClient->subscribeEntity(entityID1);
    TimeMachineObject::feedEventLoop();
    QVERIFY(!m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QVERIFY(!m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
}

void test_client_storage_event_system::subscribeToExistingEntity()
{
    addAndSubscribeToEntity(entityID1, "Foo", QVariantMap{{"Bar", QVariant(42)}, {"Baz", QVariant(37)}});
    QVERIFY(m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Bar"));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Baz"));
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "EntityName"), "Foo");
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "Bar"), 42);
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "Baz"), 37);
}

void test_client_storage_event_system::subscribeAndUnsubscribe()
{
    addAndSubscribeToEntity(entityID1, "Foo", QVariantMap{{"Bar", QVariant(42)}, {"Baz", QVariant(37)}});
    QVERIFY(m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Bar"));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Baz"));
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "EntityName"), "Foo");
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "Bar"), 42);
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "Baz"), 37);
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 1);

    m_netClient->unsubscribeEntity(entityID1);
    TimeMachineObject::feedEventLoop();
    QVERIFY(!m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 0);
}

void test_client_storage_event_system::subscribeAndEntityRemove()
{
    addAndSubscribeToEntity(entityID1, "Foo", QVariantMap{{"Bar", QVariant(42)}, {"Baz", QVariant(37)}});
    QVERIFY(m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Bar"));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Baz"));
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "EntityName"), "Foo");
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "Bar"), 42);
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "Baz"), 37);
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 1);

    m_netServer->getServer()->removeEntitiesAdded();
    TimeMachineObject::feedEventLoop();
    QVERIFY(!m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QVERIFY(!m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
    QVERIFY(!m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Bar"));
    QVERIFY(!m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Baz"));
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 0);
}

void test_client_storage_event_system::unsubscribeWithoutSubscribing()
{
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 0);
    m_netClient->unsubscribeEntity(entityID1);
    TimeMachineObject::feedEventLoop();
    QVERIFY(!m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 0);
}

void test_client_storage_event_system::fetchNonExistingComponent()
{
    addAndSubscribeToEntity(entityID1, "Foo");
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(entityID1);
    m_netClient->addItem(entityItem);
    VfClientComponentFetcherPtr componentFetcher = VfClientComponentFetcher::create("Component1", entityItem);
    entityItem->addItem(componentFetcher);

    componentFetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QVERIFY(!m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "Component1"));
}

void test_client_storage_event_system::fetchExistingComponent()
{
    addAndSubscribeToEntity(entityID1, "Foo");

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(entityID1);
    m_netClient->addItem(entityItem);
    VfClientComponentFetcherPtr componentFetcher = VfClientComponentFetcher::create("EntityName", entityItem);
    entityItem->addItem(componentFetcher);
    componentFetcher->startGetComponent();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "EntityName"), "Foo");
}

void test_client_storage_event_system::serverSetComponent()
{
    addAndSubscribeToEntity(entityID1, "Foo");
    m_netServer->getServer()->setComponentServerNotification(entityID1, "EntityName", "Bar");
    TimeMachineObject::feedEventLoop();
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "EntityName"), "Bar");
}

void test_client_storage_event_system::serverRemoveEntity()
{
    addAndSubscribeToEntity(entityID1, "Foo");
    m_netServer->getServer()->removeEntitiesAdded();
    QVERIFY(!m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
}

void test_client_storage_event_system::serverAddComponent()
{
    addAndSubscribeToEntity(entityID1, "Foo");
    m_netServer->getServer()->addComponent(entityID1, "component1", "xyz", false);
    TimeMachineObject::feedEventLoop();
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "component1"));
    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "component1"), "xyz");
}

void test_client_storage_event_system::clientSetComponent()
{
    addAndSubscribeToEntity(entityID1, "Foo");
    m_netServer->getServer()->addComponent(entityID1, "component1", "xyz", false);
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(entityID1);
    m_netClient->addItem(entityItem);
    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("component1", entityItem);
    entityItem->addItem(setter);
    setter->startSetComponent(QVariant(), "abc");
    TimeMachineObject::feedEventLoop();

    QCOMPARE(m_clientStorageSystem->getDb()->getStoredValue(entityID1, "component1"), "abc");
}

void test_client_storage_event_system::clientInvokeNonExistingRPC()
{
    int entityID = 99999;
    QJsonObject jsonSpyClient;
    TestJsonSpyEventSystem jsonSystemClient(&jsonSpyClient, "client");
    m_netClient->appendEventSystem(&jsonSystemClient);

    //vfEntityRpcEventHandler is an entity based on VfCpp
    //We append it to our server as a rpc-event handler
    vfEntityRpcEventHandler rpcEventHandler;
    m_netServer->getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());

    QJsonObject jsonSpyServer;
    TestJsonSpyEventSystem jsonSystemServer(&jsonSpyServer, "server");
    m_netServer->getServer()->appendEventSystem(&jsonSystemServer);

    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();
    m_netClient->subscribeEntity(entityID);
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr rpcInvoker = VfRPCInvoker::create(99999, std::make_unique<VfClientRPCInvoker>());
    m_netClient->addItem(rpcInvoker);

    rpcInvoker->invokeRPC("RPC_foo",QVariantMap());
    TimeMachineObject::feedEventLoop();

    QFile fileEvents(":/vein-client-storage-events/invokeIncorrectRpcEventClient.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();

    QJsonDocument doc(jsonSpyClient);
    QByteArray jsonDumped = doc.toJson();
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));

    fileEvents.setFileName(":/vein-client-storage-events/invokeIncorrectRpcEventServer.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    jsonExpected = fileEvents.readAll();

    doc = QJsonDocument(jsonSpyServer);
    jsonDumped = doc.toJson();
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_client_storage_event_system::clientInvokeExistingRpcCorrectParameter()
{
    int entityID = 99999;
    QJsonObject jsonSpyClient;
    TestJsonSpyEventSystem jsonSystemClient(&jsonSpyClient, "client");
    m_netClient->appendEventSystem(&jsonSystemClient);

    vfEntityRpcEventHandler rpcEventHandler;
    m_netServer->getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();
    m_netClient->subscribeEntity(entityID);
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr rpcInvoker = VfRPCInvoker::create(99999, std::make_unique<VfClientRPCInvoker>());
    m_netClient->addItem(rpcInvoker);

    QVariantMap parameters;
    parameters["p_param"] = false;
    rpcInvoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QFile fileEvents(":/vein-client-storage-events/invokeCorrectRpcEventClient.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();

    QJsonDocument doc(jsonSpyClient);
    QByteArray jsonDumped = doc.toJson();

    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_client_storage_event_system::clientInvokeExistingRPCWrongParameter()
{
    int entityID = 99999;
    QJsonObject jsonSpyClient;
    TestJsonSpyEventSystem jsonSystemClient(&jsonSpyClient, "client");
    m_netClient->appendEventSystem(&jsonSystemClient);

    //vfEntityRpcEventHandler is an entity based on VfCpp
    //We append it to our server as a rpc-event handler
    vfEntityRpcEventHandler rpcEventHandler;
    m_netServer->getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();
    m_netClient->subscribeEntity(entityID);
    TimeMachineObject::feedEventLoop();

    VfRPCInvokerPtr rpcInvoker = VfRPCInvoker::create(99999, std::make_unique<VfClientRPCInvoker>());
    m_netClient->addItem(rpcInvoker);

    QVariantMap parameters;
    parameters["p_param"] = 1;
    rpcInvoker->invokeRPC("RPC_forTest",parameters);
    TimeMachineObject::feedEventLoop();

    QFile fileEvents(":/vein-client-storage-events/invokeCorrectRpcWrongParamEventClient.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();

    QJsonDocument doc(jsonSpyClient);
    QByteArray jsonDumped = doc.toJson();

    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_client_storage_event_system::subscribeToNonExistingEntityRpc()
{
    m_netClient->subscribeEntity(entityID1);
    TimeMachineObject::feedEventLoop();
    QVERIFY(m_clientStorageSystem->getRpcs().isEmpty());
}

void test_client_storage_event_system::subscribeToExistingEntityWithoutRpc()
{
    addAndSubscribeToEntity(entityID1, "Foo", QVariantMap{{"Bar", QVariant(42)}, {"Baz", QVariant(37)}});
    QMap<int, QStringList> rpcs = m_clientStorageSystem->getRpcs();
    QVERIFY(rpcs.contains(entityID1));
    QCOMPARE(rpcs[entityID1], QStringList());
}

void test_client_storage_event_system::subscribeToExistingEntityWithRpc()
{
    addEntity(entityID1, "Foo", QVariantMap{{"Bar", QVariant(42)}, {"Baz", QVariant(37)}});
    vfEntityRpcEventHandler rpcEventHandler(entityID1);
    m_netServer->getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    subscribeEntity(entityID1);

    QMap<int, QStringList> rpcs = m_clientStorageSystem->getRpcs();
    QVERIFY(rpcs.contains(entityID1));
    QCOMPARE(rpcs[entityID1].count(), 2);
    QCOMPARE(rpcs[entityID1][0], "RPC_forTest(bool p_param)");
}

void test_client_storage_event_system::subscribeAndUnsubscribeWithRpc()
{
    addEntity(entityID1, "Foo", QVariantMap{{"Bar", QVariant(42)}, {"Baz", QVariant(37)}});
    vfEntityRpcEventHandler rpcEventHandler(entityID1);
    m_netServer->getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    subscribeEntity(entityID1);

    QMap<int, QStringList> rpcs = m_clientStorageSystem->getRpcs();
    QVERIFY(rpcs.contains(entityID1));
    QCOMPARE(rpcs[entityID1].count(), 2);
    QCOMPARE(rpcs[entityID1][0], "RPC_forTest(bool p_param)");

    m_netClient->unsubscribeEntity(entityID1);
    TimeMachineObject::feedEventLoop();

    rpcs = m_clientStorageSystem->getRpcs();
    QVERIFY(!rpcs.contains(entityID1));
}

void test_client_storage_event_system::subscribeAndEntityRemoveWithRpc()
{
    addEntity(entityID1, "Foo", QVariantMap{{"Bar", QVariant(42)}, {"Baz", QVariant(37)}});
    vfEntityRpcEventHandler rpcEventHandler(entityID1);
    m_netServer->getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    subscribeEntity(entityID1);

    QMap<int, QStringList> rpcs = m_clientStorageSystem->getRpcs();
    QVERIFY(rpcs.contains(entityID1));
    QCOMPARE(rpcs[entityID1].count(), 2);
    QCOMPARE(rpcs[entityID1][0], "RPC_forTest(bool p_param)");

    m_netServer->getServer()->removeEntitiesAdded();
    TimeMachineObject::feedEventLoop();

    rpcs = m_clientStorageSystem->getRpcs();
    QVERIFY(!rpcs.contains(entityID1));
}

void test_client_storage_event_system::addEntity(int entityID, const QString &entityName, const QVariantMap &components)
{
    m_netServer->getServer()->addEntity(entityID, entityName);
    for(auto iter=components.constBegin(); iter!=components.constEnd(); ++iter)
        m_netServer->getServer()->addComponent(entityID, iter.key(), iter.value(), false);
    TimeMachineObject::feedEventLoop();
}

void test_client_storage_event_system::subscribeEntity(int entityID)
{
    m_netClient->subscribeEntity(entityID);
    TimeMachineObject::feedEventLoop();
}

void test_client_storage_event_system::addAndSubscribeToEntity(int entityID,
                                                               const QString &entityName,
                                                               const QVariantMap &components)
{
    addEntity(entityID, entityName, components);
    subscribeEntity(entityID);
}
