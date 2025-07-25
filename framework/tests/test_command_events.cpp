#include "test_command_events.h"
#include "testjsonspyeventsystem.h"
#include "testloghelpers.h"
#include "vf_client_entity_unsubscriber.h"
#include "vf_server_component_add.h"
#include "vs_dumpjson.h"
#include "task_client_component_fetcher.h"
#include "task_client_component_setter.h"
#include "mocktcpnetworkfactory.h"
#include <timemachineobject.h>
#include <QBuffer>
#include <QTest>

QTEST_MAIN(test_command_events)

static constexpr int systemEntityId = 0;
static constexpr int stdTimeout = 1000;

void test_command_events::clientSubscribeEntity()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    subscribeClient(systemEntityId);

    QFile file(":/vein-event-dumps/dumpEventsSubscribe.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::clientSubscribeNonExistingEntity()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    subscribeClient(42);

    QFile file(":/vein-event-dumps/dumpEventsSubscribeNonExistent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::clientSubscribeUnsubscribeEntity()
{
    subscribeClient(systemEntityId);
    QCOMPARE(m_netServer->getSubscriberCount(systemEntityId), 1);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    VfClientEntityUnsubscriberPtr unsubscriber = VfClientEntityUnsubscriber::create(systemEntityId);
    m_netClient->addItem(unsubscriber);
    unsubscriber->sendUnsubscription();
    TimeMachineObject::feedEventLoop();

    QCOMPARE(m_netServer->getSubscriberCount(systemEntityId), 0);

    QFile file(":/vein-event-dumps/dumpEventsUnsubscribe.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::clientFetchComponent()
{
    subscribeClient(systemEntityId);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("EntityName", m_entityItem, value, stdTimeout);
    fetcherTask->start();
    TimeMachineObject::feedEventLoop();
    QCOMPARE(*value, "_System");

    QFile file(":/vein-event-dumps/dumpEventsFetch.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::clientFetchNonExistingComponent()
{
    subscribeClient(systemEntityId);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("FooComponent", m_entityItem, value, stdTimeout);
    fetcherTask->start();
    TimeMachineObject::feedEventLoop();

    QFile file1(":/vein-event-dumps/dumpEventsFetchNonExistentComponent.json");
    QVERIFY(file1.open(QFile::ReadOnly));
    QByteArray jsonExpected1 = file1.readAll();
    QByteArray jsonDumped1 = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected1, jsonDumped1));
}

void test_command_events::clientSetSytemEnttityComponent()
{
    subscribeClient(systemEntityId);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    TaskTemplatePtr setterTask = TaskClientComponentSetter::create(m_entityItem, "ModulesPaused", false, true, stdTimeout);
    setterTask->start();
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsSet.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::clientSetNonExistingComponent()
{
    subscribeClient(systemEntityId);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    TaskTemplatePtr setterTask = TaskClientComponentSetter::create(m_entityItem, "NonExisting", false, true, stdTimeout);
    setterTask->start();
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsSetNonExistent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));

    // TODO??: This time it is SystemModuleEventSystem missing error handling... How shall we test/handle
    // gazillions of error handling implementations: SystemModuleEventSystem/AbstractEventSystem/IntrospectionSystem/VfCpp/zera-classes modules???

    // Unexpected result: Server just ignores. So check if component was
    // accidentally created in server...
    VeinStorage::AbstractEventSystem* storage = m_netServer->getStorage();
    QFile file2(":/vein-storage-dumps/dumpStorageInitial.json");
    QVERIFY(file2.open(QFile::ReadOnly));

    QByteArray jsonExpected2 = file2.readAll();
    QByteArray jsonDumped2;
    QBuffer buff(&jsonDumped2);
    VeinStorage::DumpJson::dumpToFile(storage->getDb(), &buff, QList<int>());
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected2, jsonDumped2));
}

static constexpr int testEntityId = 37;
static const char* entityName = "TestEntity";
static const char* componentName = "TestComponent";
static const char* componentValue = "TestInitialValue";

void test_command_events::serverAddEntityAndComponent()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    TestVeinServer* server = m_netServer->getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsAddEntityComponent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::serverAddComponentWithSubscribedClient()
{
    TestVeinServer* server = m_netServer->getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

    subscribeClient(testEntityId);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);
    server->addComponent(testEntityId, "secondComponentName", "secondComponentInitialValue", false);
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsAddComponentClientSubscribed.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::serverAddComponentForNomExistentEntity()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    m_netServer->getServer()->sendEvent(VfServerComponentAdd::generateEvent(testEntityId, componentName, "InitialValue"));
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsAddComponentNonExistingEntity.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::serverRemoveComponent()
{
    TestVeinServer* server = m_netServer->getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    m_netServer->getServer()->removeComponent(testEntityId, componentName);
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsRemoveComponent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::serverRemoveNonExistingComponent()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(systemEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_REMOVE);
    cData->setComponentName("NonExistent");
    cData->setNewValue("InitialValue");
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    m_netServer->getServer()->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsRemoveNonExistingComponent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::serverRemoveComponentForNomExistentEntity()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(testEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_REMOVE);
    cData->setComponentName(componentName);
    cData->setNewValue("InitialValue");
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    m_netServer->getServer()->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsRemoveComponentNonExistingEntity.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::serverRemoveAllEntities()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    m_netServer->getServer()->addEntity(testEntityId, entityName);
    m_netServer->getServer()->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();
    subscribeClient(testEntityId);

    m_netServer->getServer()->removeEntitiesAdded();
    TimeMachineObject::feedEventLoop();

    QFile file(":/vein-event-dumps/dumpEventsRemoveAllEntities.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(jsonEvents);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_command_events::init()
{
    constexpr int serverPort = 4242;
    m_netServer = std::make_unique<TestVeinServerWithMockNet>(serverPort);
    m_netServer->getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    m_netClient = std::make_unique<VfCoreStackClient>(VeinTcp::MockTcpNetworkFactory::create());
    m_entityItem = VfEntityComponentEventItem::create(systemEntityId);
    m_netClient->addItem(m_entityItem);
    m_netClient->connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();
}

void test_command_events::cleanup()
{
    m_entityItem = nullptr;
    m_netClient = nullptr;
    m_netServer = nullptr;
}

void test_command_events::setupSpy(QJsonObject &jsonEvents)
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

void test_command_events::subscribeClient(int entityId)
{
    m_netClient->subscribeEntity(entityId);
    TimeMachineObject::feedEventLoop();
}
