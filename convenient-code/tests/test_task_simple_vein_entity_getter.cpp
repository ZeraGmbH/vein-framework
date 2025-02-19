#include "test_task_simple_vein_entity_getter.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include "tasksimpleveinentitygetter.h"

#include <QTest>

QTEST_MAIN(test_task_simple_vein_entity_getter)

static constexpr int serverPort = 4242;
static constexpr int systemEntityId = 0;
static constexpr int stdTimeout = 1000;

void test_task_simple_vein_entity_getter::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_simple_vein_entity_getter::getAllComponentsValid()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    serverNet.getServer()->addEntity(4711, "testEntity");
    serverNet.getServer()->addComponent(4711, "testComponent1", QVariant(1), true);
    serverNet.getServer()->addComponent(4711, "testComponent2", QVariant(0), true);
    serverNet.getServer()->addComponent(4711, "testComponent3", QVariant("foo"), true);
    serverNet.getServer()->addComponent(4711, "testComponent4", QVariant::fromValue(QList<double>() << 1 << 2 << 3), true);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinEntityGetterPtr task = TaskSimpleVeinEntityGetter::create(4711, clientStack.getCmdEventHandlerSystem(), stdTimeout);

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
    QCOMPARE(task->getValues().size(), 5);
    QCOMPARE(*task->getValues()[0], "testEntity");
    QCOMPARE(*task->getValues()[1], 1);
    QCOMPARE(*task->getValues()[2], 0);
    QCOMPARE(*task->getValues()[3], "foo");
    QCOMPARE(task->getValues()[4]->value<QList<double>>()[0], 1);
}

void test_task_simple_vein_entity_getter::getInvalidEntity()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinEntityGetterPtr task = TaskSimpleVeinEntityGetter::create(4711, clientStack.getCmdEventHandlerSystem(), stdTimeout);

    bool receivedOk = false;
    int timeout=0;
    connect(task.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(!receivedOk);
}

void test_task_simple_vein_entity_getter::fireSameTaskTwiceAfterAnother()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    serverNet.getServer()->addEntity(4711, "testEntity");
    serverNet.getServer()->addComponent(4711, "testComponent1", QVariant(1), true);
    serverNet.getServer()->addComponent(4711, "testComponent2", QVariant(0), true);
    serverNet.getServer()->addComponent(4711, "testComponent3", QVariant("foo"), true);
    serverNet.getServer()->addComponent(4711, "testComponent4", QVariant::fromValue(QList<double>() << 1 << 2 << 3), true);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinEntityGetterPtr task = TaskSimpleVeinEntityGetter::create(4711, clientStack.getCmdEventHandlerSystem(), stdTimeout);
    TaskSimpleVeinEntityGetterPtr taskTwo = TaskSimpleVeinEntityGetter::create(4711, clientStack.getCmdEventHandlerSystem(), stdTimeout);

    bool receivedOkTask = false;
    int timeout=0;
    connect(task.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOkTask = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });

    bool receivedOkTaskTwo = false;
    timeout=0;
    connect(taskTwo.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOkTaskTwo = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });

    task->start();

    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    taskTwo->start();

    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(receivedOkTask);
    QCOMPARE(task->getValues().size(), 5);
    QCOMPARE(*task->getValues()[0], "testEntity");
    QCOMPARE(*task->getValues()[1], 1);
    QCOMPARE(*task->getValues()[2], 0);
    QCOMPARE(*task->getValues()[3], "foo");
    QCOMPARE(task->getValues()[4]->value<QList<double>>()[0], 1);

    QVERIFY(receivedOkTaskTwo);
    QCOMPARE(taskTwo->getValues().size(), 5);
    QCOMPARE(*taskTwo->getValues()[0], "testEntity");
    QCOMPARE(*taskTwo->getValues()[1], 1);
    QCOMPARE(*taskTwo->getValues()[2], 0);
    QCOMPARE(*taskTwo->getValues()[3], "foo");
    QCOMPARE(taskTwo->getValues()[4]->value<QList<double>>()[0], 1);
}

void test_task_simple_vein_entity_getter::getEntityWithInvalidComponent()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinEntityGetterPtr task = TaskSimpleVeinEntityGetter::create(systemEntityId, clientStack.getCmdEventHandlerSystem(), stdTimeout);

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
}

void test_task_simple_vein_entity_getter::fireSameTaskTwiceSameTime()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    serverNet.getServer()->addEntity(4711, "testEntity");
    serverNet.getServer()->addComponent(4711, "testComponent1", QVariant(1), true);
    serverNet.getServer()->addComponent(4711, "testComponent2", QVariant(0), true);
    serverNet.getServer()->addComponent(4711, "testComponent3", QVariant("foo"), true);
    serverNet.getServer()->addComponent(4711, "testComponent4", QVariant::fromValue(QList<double>() << 1 << 2 << 3), true);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);

    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinEntityGetterPtr task = TaskSimpleVeinEntityGetter::create(4711, clientStack.getCmdEventHandlerSystem(), stdTimeout);
    TaskSimpleVeinEntityGetterPtr taskTwo = TaskSimpleVeinEntityGetter::create(4711, clientStack.getCmdEventHandlerSystem(), stdTimeout);

    bool receivedOkTask = false;
    int timeout=0;
    connect(task.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOkTask = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });

    bool receivedOkTaskTwo = false;
    timeout=0;
    connect(taskTwo.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOkTaskTwo = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });

    task->start();
    taskTwo->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(receivedOkTask);
    QCOMPARE(task->getValues().size(), 5);
    QCOMPARE(*task->getValues()[0], "testEntity");
    QCOMPARE(*task->getValues()[1], 1);
    QCOMPARE(*task->getValues()[2], 0);
    QCOMPARE(*task->getValues()[3], "foo");
    QCOMPARE(task->getValues()[4]->value<QList<double>>()[0], 1);

    QVERIFY(receivedOkTaskTwo);
    QCOMPARE(taskTwo->getValues().size(), 5);
    QCOMPARE(*taskTwo->getValues()[0], "testEntity");
    QCOMPARE(*taskTwo->getValues()[1], 1);
    QCOMPARE(*taskTwo->getValues()[2], 0);
    QCOMPARE(*taskTwo->getValues()[3], "foo");
    QCOMPARE(taskTwo->getValues()[4]->value<QList<double>>()[0], 1);
}
