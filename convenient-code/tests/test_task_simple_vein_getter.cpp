#include "test_task_simple_vein_getter.h"
#include "tasksimpleveingetter.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_simple_vein_getter)

static constexpr int serverPort = 4242;
static constexpr int systemEntityId = 0;
static constexpr int stdTimeout = 1000;

void test_task_simple_vein_getter::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_simple_vein_getter::getValid()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinGetterPtr task = TaskSimpleVeinGetter::create(systemEntityId, "EntityName",
                                                                clientStack.getCmdEventHandlerSystem(), stdTimeout);
    bool receivedOk = false;
    int timeout=0;
    connect(task.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(receivedOk);
    QCOMPARE(task->getValue(), "_System");
}

void test_task_simple_vein_getter::getInvalid()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinGetterPtr task = TaskSimpleVeinGetter::create(systemEntityId, "foo",
                                                                clientStack.getCmdEventHandlerSystem(), stdTimeout);
    QSignalSpy spy(task.get(), &TaskTemplate::sigFinish);
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].toBool(), false);
}

void test_task_simple_vein_getter::getTimeout()
{
    VfCmdEventHandlerSystemPtr cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    TaskSimpleVeinGetterPtr task = TaskSimpleVeinGetter::create(systemEntityId, "foo",
                                                                              cmdEventHandlerSystem, stdTimeout);
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

void test_task_simple_vein_getter::getEntities()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinGetterPtr task = TaskSimpleVeinGetter::create(systemEntityId, "Entities",
                                                                clientStack.getCmdEventHandlerSystem(), stdTimeout);
    bool receivedOk = false;
    int timeout=0;
    connect(task.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(receivedOk);
    QCOMPARE(task->getValue(), "");
}
