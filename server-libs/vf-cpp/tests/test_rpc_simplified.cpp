#include "test_rpc_simplified.h"
#include "testveinserverwithmocknet.h"
#include "vftestrpcsimplified.h"
#include <timemachinefortest.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_rpc_simplified)

static constexpr int serverPort = 4242;
static constexpr int entityIdWithRpc = 1111;

void test_rpc_simplified::callRpcValidParam()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    VfTestRpcSimplified additionalEntityWithRpc(entityIdWithRpc);
    serverNet.getServer()->appendEventSystem(additionalEntityWithRpc.getEntity());
    additionalEntityWithRpc.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy spyRpcFinish(serverNet.getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", 72);
    serverNet.getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap["RemoteProcedureData::Return"];
    QCOMPARE(resultData, "72");
}

void test_rpc_simplified::callRpcInvalidParamValue()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    VfTestRpcSimplified additionalEntityWithRpc(entityIdWithRpc);
    serverNet.getServer()->appendEventSystem(additionalEntityWithRpc.getEntity());
    additionalEntityWithRpc.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy spyRpcFinish(serverNet.getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", -72);
    serverNet.getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_errorMessageString];
    QCOMPARE(resultData, "Error");
}

void test_rpc_simplified::callRpcInvalidParamName()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    VfTestRpcSimplified additionalEntityWithRpc(entityIdWithRpc);
    serverNet.getServer()->appendEventSystem(additionalEntityWithRpc.getEntity());
    additionalEntityWithRpc.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy spyRpcFinish(serverNet.getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("foo", 72);
    serverNet.getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}

void test_rpc_simplified::callRpcMissingParam()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    VfTestRpcSimplified additionalEntityWithRpc(entityIdWithRpc);
    serverNet.getServer()->appendEventSystem(additionalEntityWithRpc.getEntity());
    additionalEntityWithRpc.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy spyRpcFinish(serverNet.getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    serverNet.getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}

void test_rpc_simplified::callInvalidRpc()
{
    TestVeinServerWithMockNet serverNet(serverPort);
    VfTestRpcSimplified additionalEntityWithRpc(entityIdWithRpc);
    serverNet.getServer()->appendEventSystem(additionalEntityWithRpc.getEntity());
    additionalEntityWithRpc.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy spyRpcFinish(serverNet.getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", -72);
    serverNet.getServer()->clientInvokeRpc(entityIdWithRpc, "foo", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}
