#include "test_task_simple_vein_setter.h"
#include "tasksimpleveinsetter.h"
#include "tasksimpleveingetter.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_simple_vein_setter)

static constexpr int serverPort = 4242;
static constexpr int testId = 1;
static constexpr int stdTimeout = 1000;

void test_task_simple_vein_setter::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_simple_vein_setter::setValid()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverNet.getServer()->appendEventSystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinSetterPtr taskSet = TaskSimpleVeinSetter::create(testId, "foo", 4711,
                                                                   clientStack.getCmdEventHandlerSystem(), stdTimeout);
    bool receivedOk = false;
    int timeout=0;
    connect(taskSet.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    taskSet->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(receivedOk);


    TaskSimpleVeinGetterPtr taskGet = TaskSimpleVeinGetter::create(testId, "foo",
                                                                   clientStack.getCmdEventHandlerSystem(), stdTimeout);
    receivedOk = false;
    timeout=0;
    connect(taskGet.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    taskGet->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(receivedOk);
    QCOMPARE(taskGet->getValue(), 4711);
}

void test_task_simple_vein_setter::setInvalid()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverNet.getServer()->appendEventSystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinSetterPtr taskSet = TaskSimpleVeinSetter::create(testId, "bar", 4711,
                                                                   clientStack.getCmdEventHandlerSystem(), stdTimeout);
    QSignalSpy spy(taskSet.get(), &TaskTemplate::sigFinish);
    taskSet->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].toBool(), false);
}

void test_task_simple_vein_setter::setTimeout()
{
    VfCmdEventHandlerSystemPtr cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    TaskSimpleVeinSetterPtr taskSet = TaskSimpleVeinSetter::create(testId, "bar", 4711,
                                                                                 cmdEventHandlerSystem, stdTimeout);
    bool receivedOk = true;
    int timeout=0;
    connect(taskSet.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    taskSet->start();
    TimeMachineForTest::getInstance()->processTimers(stdTimeout);

    QVERIFY(!receivedOk);
    QCOMPARE(timeout, stdTimeout);
}
