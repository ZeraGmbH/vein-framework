#include "test_task_client_entity_subscribe.h"
#include "task_client_entity_subscribe.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_client_entity_subscribe)

static constexpr int serverPort = 4242;
static constexpr int systemEntityId = 0;
static constexpr int invalidEntityId = 1;
static constexpr int stdTimeout = 1000;

void test_task_client_entity_subscribe::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_client_entity_subscribe::subscibeOk()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    std::shared_ptr<QStringList> components = std::make_shared<QStringList>();
    TaskClientEntitySubscribe task(systemEntityId, clientStack.getCmdEventHandlerSystem(), components);
    QSignalSpy spy(&task, &TaskTemplate::sigFinish);
    task.start();
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QVERIFY(components->size() > 0);
}

void test_task_client_entity_subscribe::timeout()
{
    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    TimeMachineObject::feedEventLoop();

    std::shared_ptr<QStringList> components = std::make_shared<QStringList>();
    TaskTemplatePtr task = TaskClientEntitySubscribe::create(systemEntityId, clientStack.getCmdEventHandlerSystem(), components, stdTimeout);

    bool receivedOk = true;
    int timeout=0;
    connect(task.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);
    QVERIFY(!receivedOk);
    QCOMPARE(timeout, stdTimeout);
}

void test_task_client_entity_subscribe::invalidEntity()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    std::shared_ptr<QStringList> components = std::make_shared<QStringList>();
    TaskTemplatePtr task = TaskClientEntitySubscribe::create(invalidEntityId, clientStack.getCmdEventHandlerSystem(), components, stdTimeout);

    bool receivedOk = true;
    int timeout=0;
    connect(task.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);
    QVERIFY(!receivedOk);
    QCOMPARE(timeout, stdTimeout);
}
