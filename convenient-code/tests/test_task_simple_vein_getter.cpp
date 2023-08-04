#include "test_task_simple_vein_getter.h"
#include "tasksimpleveingetter.h"
#include "vf_core_stack_client.h"
#include "vf_test_server_stack.h"
#include "vtcp_workerfactorymethodstest.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include <QAbstractEventDispatcher>
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);

    feedEventLoop();

    std::unique_ptr<TaskSimpleVeinGetter> task = TaskSimpleVeinGetter::create(systemEntityId, "EntityName",
                                                                              clientStack.cmdEventHandlerSystem, stdTimeout);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);

    feedEventLoop();

    std::unique_ptr<TaskSimpleVeinGetter> task = TaskSimpleVeinGetter::create(systemEntityId, "foo",
                                                                              clientStack.cmdEventHandlerSystem, stdTimeout);
    QSignalSpy spy(task.get(), &TaskTemplate::sigFinish);
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].toBool(), false);
}

void test_task_simple_vein_getter::getTimeout()
{
    VfCmdEventHandlerSystemPtr cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    std::unique_ptr<TaskSimpleVeinGetter> task = TaskSimpleVeinGetter::create(systemEntityId, "foo",
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

void test_task_simple_vein_getter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
