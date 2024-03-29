#include "test_command_events.h"
#include "testcommandeventspyeventsystem.h"
#include "testdumpreporter.h"
#include "task_client_component_fetcher.h"
#include "vtcp_workerfactorymethodstest.h"
#include <timemachineobject.h>
#include <QTest>

QTEST_MAIN(test_command_events)

static constexpr int systemEntityId = 0;
static constexpr int stdTimeout = 1000;

void test_command_events::subscribeSystemEntity()
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

void test_command_events::fetchSystemEntityComponent()
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

void test_command_events::initTestCase()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
}

void test_command_events::init()
{
    constexpr int serverPort = 4242;
    m_netServer = std::make_unique<TestVeinServerWithNet>(serverPort);
    m_netServer->getServer()->simulAllModulesLoaded(QString("foo"), QStringList() << "fooList");

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
    m_netServer->getServer()->appendEventSystem(m_serverCmdEventSpy.get());
    m_clientCmdEventSpy = std::make_unique<TestCommandEventSpyEventSystem>(&jsonEvents, "client");
    m_netClient->appendEventSystem(m_clientCmdEventSpy.get());
}

void test_command_events::subscribeClient(int entityId)
{
    m_netClient->subscribeEntity(entityId);
    TimeMachineObject::feedEventLoop();
}
