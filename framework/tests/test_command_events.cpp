#include "test_command_events.h"
#include "testcommandeventspyeventsystem.h"
#include "testdumpreporter.h"
#include "vf_client_entity_unsubscriber.h"
#include "task_client_component_fetcher.h"
#include "task_client_component_setter.h"
#include "vtcp_workerfactorymethodstest.h"
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

    QFile file(":/dumpEventsSubscribe.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
}

void test_command_events::clientSubscribeNonExistingEntity()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    subscribeClient(42);

    QFile file(":/dumpEventsSubscribeNonExistent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
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

    // * NetworkSystemPrivate::processCmdEvents accepts unsubscribe event so we see no events from server
    // * Unsunscribe is a fire and forget so testing unsubscribe on invalid or not sunscribed is a
    //   mission impossible
    QFile file(":/dumpEventsUnsubscribe.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
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

    QFile file(":/dumpEventsFetch.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
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

    QFile file1(":/dumpEventsFetchNonExistentComponent.json");
    QVERIFY(file1.open(QFile::ReadOnly));
    QByteArray jsonExpected1 = file1.readAll();
    QByteArray jsonDumped1 = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected1, jsonDumped1));
}

void test_command_events::clientSetComponent()
{
    subscribeClient(systemEntityId);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    TaskTemplatePtr setterTask = TaskClientComponentSetter::create(m_entityItem, "ModulesPaused", false, true, stdTimeout);
    setterTask->start();
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpEventsSet.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
}

void test_command_events::clientSetNonExistingComponent()
{
    subscribeClient(systemEntityId);

    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    TaskTemplatePtr setterTask = TaskClientComponentSetter::create(m_entityItem, "NonExisting", false, true, stdTimeout);
    setterTask->start();
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpEventsSetNonExistent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));

    // Unexpected result: Server just ignores. So check if component was
    // accidentally created in server...
    VeinEvent::StorageSystem* storage = m_netServer->getStorage();
    QFile file2(":/dumpStorageInitial.json");
    QVERIFY(file2.open(QFile::ReadOnly));

    QByteArray jsonExpected2 = file2.readAll();
    QByteArray jsonDumped2;
    QBuffer buff(&jsonDumped2);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected2, jsonDumped2));
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

    QFile file(":/dumpEventsAddEntityComponent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
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

    QFile file(":/dumpEventsAddComponentClientSubscribed.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
}

void test_command_events::serverAddComponentForNomExistentEntity()
{
    QJsonObject jsonEvents;
    setupSpy(jsonEvents);

    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(testEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_ADD);
    cData->setComponentName(componentName);
    cData->setNewValue("InitialValue");
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    m_netServer->getServer()->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpEventsAddComponentNonExistingEntity.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
}

void test_command_events::serverRemoveComponent()
{
    TestVeinServer* server = m_netServer->getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

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
    server->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpEventsRemoveComponent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
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

    QFile file(":/dumpEventsRemoveNonExistingComponent.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
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

    QFile file(":/dumpEventsRemoveComponentNonExistingEntity.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestDumpReporter::dump(jsonEvents);
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
}

void test_command_events::initTestCase()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
}

void test_command_events::init()
{
    constexpr int serverPort = 4242;
    m_netServer = std::make_unique<TestVeinServerWithNet>(serverPort);
    m_netServer->getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    m_netClient = std::make_unique<VfCoreStackClient>();
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
    m_serverCmdEventSpy = std::make_unique<TestCommandEventSpyEventSystem>(&jsonEvents, "server");
    connect(m_netServer->getServer()->getEventHandler(), &VeinEvent::EventHandler::sigEventAccepted,
            m_serverCmdEventSpy.get(), &TestCommandEventSpyEventSystem::onEventAccepted);
    m_netServer->getServer()->appendEventSystem(m_serverCmdEventSpy.get());
    m_clientCmdEventSpy = std::make_unique<TestCommandEventSpyEventSystem>(&jsonEvents, "client");
    m_netClient->appendEventSystem(m_clientCmdEventSpy.get());
}

void test_command_events::subscribeClient(int entityId)
{
    m_netClient->subscribeEntity(entityId);
    TimeMachineObject::feedEventLoop();
}
