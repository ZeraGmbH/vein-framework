#include "test_task_simple_vein_rpc_invoker.h"
#include "tasksimpleveinrpcinvoker.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include "vf_entity_rpc_event_handler.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_task_simple_vein_rpc_invoker)

static constexpr int serverPort = 4242;
static constexpr int rpcHandlerId = 99999;
static constexpr int stdTimeout = 1000;

void test_task_simple_vein_rpc_invoker::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_simple_vein_rpc_invoker::getValid()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    //vfEntityRpcEventHandler is an entity based on VfCpp
    //We append it to our server as a rpc-event handler
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    QVariantMap parameters;
    parameters["p_param"] = false;
    std::shared_ptr<bool> rpcSuccessful = std::make_shared<bool>();
    std::shared_ptr<QVariant> result = std::make_shared<QVariant>();
    TaskSimpleVeinRPCInvokerPtr task = TaskSimpleVeinRPCInvoker::create(rpcHandlerId, "RPC_forTest",parameters, rpcSuccessful, result,
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
    QVERIFY(rpcSuccessful);
    QCOMPARE(result->toBool(), true);
}

void test_task_simple_vein_rpc_invoker::getInvalid()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    vfEntityRpcEventHandler rpcEventHandler;
    serverNet.getServer()->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    QVariantMap parameters;
    parameters["p_param"] = false;
    TaskSimpleVeinRPCInvokerPtr task = TaskSimpleVeinRPCInvoker::create(rpcHandlerId, "RPC_foo(bool p_param)",parameters, std::make_shared<bool>(),
                                                                        std::make_shared<QVariant>(), clientStack.getCmdEventHandlerSystem(), stdTimeout);
    QSignalSpy spy(task.get(), &TaskTemplate::sigFinish);
    task->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].toBool(), false);
}

void test_task_simple_vein_rpc_invoker::getTimeout()
{
    VfCmdEventHandlerSystemPtr cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    QVariantMap parameters;
    parameters["p_param"] = true;
    TaskSimpleVeinRPCInvokerPtr task = TaskSimpleVeinRPCInvoker::create(rpcHandlerId, "RPC_foo(bool p_param)",parameters, std::make_shared<bool>(),
                                                                        std::make_shared<QVariant>(), cmdEventHandlerSystem, stdTimeout);

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
