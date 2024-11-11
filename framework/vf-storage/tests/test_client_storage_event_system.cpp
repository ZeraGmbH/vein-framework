#include "test_client_storage_event_system.h"
#include "mocktcpnetworkfactory.h"
#include "vf_client_component_fetcher.h"
#include "vf_client_component_setter.h"
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
    addAndSubscribeToEntity(entityID1, "Foo");
    QVERIFY(m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
}

void test_client_storage_event_system::subscribeAndUnsubscribe()
{
    addAndSubscribeToEntity(entityID1, "Foo");
    QVERIFY(m_clientStorageSystem->getDb()->hasEntity(entityID1));
    QVERIFY(m_clientStorageSystem->getDb()->hasStoredValue(entityID1, "EntityName"));
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 1);

    m_netClient->unsubscribeEntity(entityID1);
    TimeMachineObject::feedEventLoop();
    //no response event reaches to m_clientStorageSystem
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 0);
}

void test_client_storage_event_system::unsubscribeWithoutSubscribing()
{
    QCOMPARE(m_netServer->getSubscriberCount(entityID1), 0);
    m_netClient->unsubscribeEntity(entityID1);
    TimeMachineObject::feedEventLoop();
    //no response event reaches to m_clientStorageSystem
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

void test_client_storage_event_system::addAndSubscribeToEntity(int entityID, QString entityName)
{
    m_netServer->getServer()->addEntity(entityID, entityName);
    QSignalSpy spySubscription(m_netClient.get(), &VfCoreStackClient::sigSubscribed);
    m_netClient->subscribeEntity(entityID);
    TimeMachineObject::feedEventLoop();
}
