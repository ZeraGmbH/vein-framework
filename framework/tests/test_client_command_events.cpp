#include "test_client_command_events.h"
#include "testcommandeventspyeventsystem.h"
#include "vf_core_stack_client.h"
#include "task_client_component_fetcher.h"
#include "vtcp_workerfactorymethodstest.h"
#include <timemachineobject.h>
#include <QTest>

QTEST_MAIN(test_client_command_events)

static constexpr int serverPort = 4242;
static constexpr int systemEntityId = 0;
static constexpr int stdTimeout = 1000;

void test_client_command_events::initTestCase()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
}

void test_client_command_events::init()
{
    m_netServer = std::make_unique<TestVeinServerWithNet>(serverPort);
    m_netServer->getServer()->simulAllModulesLoaded(QString("foo"), QStringList() << "fooList");
}

void test_client_command_events::cleanup()
{
    m_netServer = nullptr;
}

void test_client_command_events::subscribeSystemEntity()
{
    VfCoreStackClient vfClient;
    vfClient.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    QJsonObject jsonEvents;
    TestCommandEventSpyEventSystem serverCmdEventSpy(&jsonEvents, "server");
    m_netServer->getServer()->appendEventSystem(&serverCmdEventSpy);
    TestCommandEventSpyEventSystem clientCmdEventSpy(&jsonEvents, "client");
    vfClient.appendEventSystem(&clientCmdEventSpy);

    vfClient.subscribeEntity(systemEntityId);
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpEventsSubscribe.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = clientCmdEventSpy.dumpToJson();
    if(jsonExpected != jsonDumped) {
        qWarning("Expected events:");
        qInfo("%s", qPrintable(jsonExpected));
        qWarning("Dumped events:");
        qInfo("%s", qPrintable(jsonDumped));
        QCOMPARE(jsonExpected, jsonDumped);
    }
}

void test_client_command_events::fetchSystemEntity()
{
    VfCoreStackClient vfClient;
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    vfClient.addItem(entityItem);
    vfClient.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    vfClient.subscribeEntity(systemEntityId);
    TimeMachineObject::feedEventLoop();

    QJsonObject jsonEvents;
    TestCommandEventSpyEventSystem serverCmdEventSpy(&jsonEvents, "server");
    m_netServer->getServer()->appendEventSystem(&serverCmdEventSpy);
    TestCommandEventSpyEventSystem clientCmdEventSpy(&jsonEvents, "client");
    vfClient.appendEventSystem(&clientCmdEventSpy);

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("EntityName", entityItem, value, stdTimeout);
    fetcherTask->start();
    TimeMachineObject::feedEventLoop();
    QCOMPARE(*value, "_System");

    QFile file(":/dumpEventsFetch.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = clientCmdEventSpy.dumpToJson();
    if(jsonExpected != jsonDumped) {
        qWarning("Expected events:");
        qInfo("%s", qPrintable(jsonExpected));
        qWarning("Dumped events:");
        qInfo("%s", qPrintable(jsonDumped));
        QCOMPARE(jsonExpected, jsonDumped);
    }
}
