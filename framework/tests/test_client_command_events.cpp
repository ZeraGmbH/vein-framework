#include "test_client_command_events.h"
#include "testcommandeventspyeventsystem.h"
#include "testveinserverwithnet.h"
#include "vf_core_stack_client.h"
#include "vtcp_workerfactorymethodstest.h"
#include <timemachineobject.h>
#include <QTest>

QTEST_MAIN(test_client_command_events)

static constexpr int serverPort = 4242;

void test_client_command_events::initTestCase()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
}

void test_client_command_events::subscribeSystemEntity()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded(QString("foo"), QStringList() << "fooList");

    VfCoreStackClient client;
    TestCommandEventSpyEventSystem cmdEventSpy;
    client.appendEventSystem(&cmdEventSpy);
    client.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    QVERIFY(cmdEventSpy.isEmpty());

    client.subscribeEntity(0);
    TimeMachineObject::feedEventLoop();

    QFile file(":/dumpSubscribe.json");
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


