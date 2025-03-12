#include "test_task_client_rpc_invoker.h"
#include "task_client_entity_subscribe.h"
#include "task_client_rpc_invoker.h"
#include "testveinserverwithmocknet.h"
#include "vf_core_stack_client.h"
#include "vf_entity_rpc_event_handler.h"
#include "mocktcpnetworkfactory.h"
#include "timerfactoryqtfortest.h"
#include "timemachinefortest.h"
#include <QSignalSpy>
#include <QTest>

static constexpr int serverPort = 4242;
static constexpr int rpcHandlerId = 99999;
static constexpr int stdTimeout = 1000;

QTEST_MAIN(test_task_client_rpc_invoker)

void test_task_client_rpc_invoker::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();
}

void test_task_client_rpc_invoker::invokeSubscribedRPC()
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

    TaskClientEntitySubscribe taskSubscribe(rpcHandlerId, clientStack.getCmdEventHandlerSystem(), std::make_shared<QStringList>());
    taskSubscribe.start();
    TimeMachineObject::feedEventLoop();

    QVariantMap parameters;
    parameters["p_param"] = true;
    std::shared_ptr<QVariant> result = std::make_shared<QVariant>();
    TaskTemplatePtr taskInvoker = TaskClientRPCInvoker::create(rpcHandlerId, "RPC_forTest(bool p_param)",parameters, result, clientStack.getCmdEventHandlerSystem(), stdTimeout);
    QSignalSpy spy(taskInvoker.get(), &TaskTemplate::sigFinish);
    taskInvoker->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].toBool(), true);
}

void test_task_client_rpc_invoker::timeout()
{
    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    TimeMachineObject::feedEventLoop();

    QVariantMap parameters;
    parameters["p_param"] = true;
    std::shared_ptr<QVariant> result = std::make_shared<QVariant>();
    TaskTemplatePtr taskInvoker = TaskClientRPCInvoker::create(rpcHandlerId, "RPC_forTest(bool p_param)",parameters, result, clientStack.getCmdEventHandlerSystem(), stdTimeout);

    bool receivedOk = true;
    int timeout = 0;
    connect(taskInvoker.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId) {
        Q_UNUSED(taskId)
        receivedOk = ok;
        timeout = TimeMachineForTest::getInstance()->getCurrentTimeMs();
    });
    taskInvoker->start();
    TimeMachineForTest::getInstance()->processTimers(2*stdTimeout);
    QVERIFY(!receivedOk);
    QCOMPARE(timeout, stdTimeout);

}
