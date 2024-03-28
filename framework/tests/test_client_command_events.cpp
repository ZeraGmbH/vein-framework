#include "test_client_command_events.h"
#include "testcommandeventspyeventsystem.h"
#include "testveinserverwithnet.h"
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

void test_client_command_events::subscribeSystemEntity()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded(QString("foo"), QStringList() << "fooList");

    VfCoreStackClient client;
    QJsonObject jsonEvents;
    TestCommandEventSpyEventSystem cmdEventSpy(&jsonEvents);
    client.appendEventSystem(&cmdEventSpy);
    client.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    QVERIFY(cmdEventSpy.isEmpty());

    client.subscribeEntity(systemEntityId);
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpEventsSubscribe.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = cmdEventSpy.dumpToJson();
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
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded(QString("foo"), QStringList() << "fooList");

    VfCoreStackClient client;
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    client.addItem(entityItem);
    QJsonObject jsonEvents;
    TestCommandEventSpyEventSystem cmdEventSpy(&jsonEvents);
    client.appendEventSystem(&cmdEventSpy);
    client.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    client.subscribeEntity(systemEntityId);
    TimeMachineObject::feedEventLoop();
    cmdEventSpy.clear();

    std::shared_ptr<QVariant> value = std::make_shared<QVariant>();
    TaskTemplatePtr fetcherTask = TaskClientComponentFetcher::create("EntityName", entityItem, value, stdTimeout);
    fetcherTask->start();
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpEventsFetch.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = cmdEventSpy.dumpToJson();
    if(jsonExpected != jsonDumped) {
        qWarning("Expected events:");
        qInfo("%s", qPrintable(jsonExpected));
        qWarning("Dumped events:");
        qInfo("%s", qPrintable(jsonDumped));
        QCOMPARE(jsonExpected, jsonDumped);
    }
}


