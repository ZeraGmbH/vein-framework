#include "test_task_simple_vein_setter.h"
#include "tasksimpleveinsetter.h"
#include "tasksimpleveingetter.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithnet.h"
#include "vtcp_workerfactorymethodstest.h"
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestVeinServerWithNet serverNet(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverNet.getEventHandler()->addSubsystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinSetterPtr taskSet = TaskSimpleVeinSetter::create(testId, "foo", 4711,
                                                                              clientStack.cmdEventHandlerSystem, stdTimeout);
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
                                                                              clientStack.cmdEventHandlerSystem, stdTimeout);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestVeinServerWithNet serverNet(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverNet.getEventHandler()->addSubsystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    TimeMachineObject::feedEventLoop();

    TaskSimpleVeinSetterPtr taskSet = TaskSimpleVeinSetter::create(testId, "bar", 4711,
                                                                                 clientStack.cmdEventHandlerSystem, stdTimeout);
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
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QVERIFY(!receivedOk);
    QCOMPARE(timeout, 2*stdTimeout); // TaskSimpleVeinSetter is butchered to run a second task inside which will create a "double" timeout
}
