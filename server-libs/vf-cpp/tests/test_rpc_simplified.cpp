#include "test_rpc_simplified.h"
#include "vftestrpcsimplified.h"
#include <timemachinefortest.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_rpc_simplified)

static constexpr int serverPort = 4242;
static constexpr int entityIdWithRpc = 1111;

void test_rpc_simplified::init()
{
    m_serverNet = std::make_unique<TestVeinServerWithMockNet>(serverPort);
    m_additionalEntityWithRpc = std::make_unique<VfTestRpcSimplified>(entityIdWithRpc);
    m_serverNet->getServer()->appendEventSystem(m_additionalEntityWithRpc->getEntity());
    m_additionalEntityWithRpc->initOnce();
    TimeMachineObject::feedEventLoop();
}

void test_rpc_simplified::cleanup()
{
    m_additionalEntityWithRpc = nullptr;
    m_serverNet = nullptr;
}

void test_rpc_simplified::callRpcValidParam()
{
    QSignalSpy spyRpcFinish(m_serverNet->getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", 72);
    m_serverNet->getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap["RemoteProcedureData::Return"];
    QCOMPARE(resultData, "72");
}

void test_rpc_simplified::callRpcInvalidParamValue()
{
    QSignalSpy spyRpcFinish(m_serverNet->getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", -72);
    m_serverNet->getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_errorMessageString];
    QCOMPARE(resultData, "Error");
}

void test_rpc_simplified::callRpcInvalidParamName()
{
    QSignalSpy spyRpcFinish(m_serverNet->getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("foo", 72);
    m_serverNet->getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}

void test_rpc_simplified::callRpcMissingParam()
{
    QSignalSpy spyRpcFinish(m_serverNet->getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    m_serverNet->getServer()->clientInvokeRpc(entityIdWithRpc, "RPC_forTest", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}

void test_rpc_simplified::callInvalidRpc()
{
    QSignalSpy spyRpcFinish(m_serverNet->getServer(), &TestVeinServer::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", -72);
    m_serverNet->getServer()->clientInvokeRpc(entityIdWithRpc, "foo", rpcParams);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}
