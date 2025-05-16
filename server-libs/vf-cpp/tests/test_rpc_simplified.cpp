#include "test_rpc_simplified.h"
#include "mocktcpnetworkfactory.h"
#include <timerfactoryqtfortest.h>
#include <timemachinefortest.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_rpc_simplified)

static constexpr int serverPort = 4242;
static constexpr int entityIdWithRpc = 1111;

void test_rpc_simplified::init()
{
    TimerFactoryQtForTest::enableTest();
    TimeMachineForTest::reset();

    m_serverNet = std::make_unique<TestVeinServerWithMockNet>(serverPort);
    m_additionalEntityWithRpc = std::make_unique<VfEntityWithRpcSimplified>(entityIdWithRpc);
    m_serverNet->getServer()->appendEventSystem(m_additionalEntityWithRpc.get());
    m_additionalEntityWithRpc->initModule();
    TimeMachineObject::feedEventLoop();

    m_clientStack = std::make_unique<VfCoreStackClient>(VeinTcp::MockTcpNetworkFactory::create());
    m_clientStack->connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();
    m_clientStack->subscribeEntity(entityIdWithRpc);
    TimeMachineObject::feedEventLoop();

    m_rpcInvoker = std::make_shared<VfClientRPCInvoker>(entityIdWithRpc);
    m_clientStack->addItem(m_rpcInvoker);
}

void test_rpc_simplified::cleanup()
{
    m_rpcInvoker = nullptr;
    m_clientStack = nullptr;
    m_additionalEntityWithRpc = nullptr;
    m_serverNet = nullptr;
}

void test_rpc_simplified::callRpcValidParam()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "p_param", 72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QVERIFY(isRpcFound(spyRpcFinish[0]));
    QCOMPARE(getReturnResult(spyRpcFinish[0]), "72");
}

void test_rpc_simplified::callRpcInvalidParamValue()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "p_param", -72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QVERIFY(isRpcFound(spyRpcFinish[0]));
    QCOMPARE(getReturnError(spyRpcFinish[0]), "Error");
}

void test_rpc_simplified::callRpcInvalidParamName()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "foo", 72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QVERIFY(!isRpcFound(spyRpcFinish[0]));
}

void test_rpc_simplified::callRpcMissingParam()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "", "");
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QVERIFY(!isRpcFound(spyRpcFinish[0]));
}

void test_rpc_simplified::callInvalidRpc()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    invokeRpc("foo", "p_param", -72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QVERIFY(!isRpcFound(spyRpcFinish[0]));
}

void test_rpc_simplified::callRPCTwice()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QUuid id1 = invokeRpc("RPC_forTest", "p_param", 72);
    QUuid id2 = invokeRpc("RPC_forTest", "p_param", 48);
    QCOMPARE(spyRpcFinish.count(), 2);

    QCOMPARE(spyRpcFinish[0][1].toUuid(), id1);
    QCOMPARE(getReturnResult(spyRpcFinish[0]), "72");
    QCOMPARE(spyRpcFinish[1][1].toUuid(), id2);
    QCOMPARE(getReturnResult(spyRpcFinish[1]), "48");
}

void test_rpc_simplified::callRPCRespondingAfterDelay()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", 72);
    rpcParams.insert("p_delayMs", 5000);
    QUuid id1 = m_rpcInvoker->invokeRPC("RPC_addDelay", rpcParams);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spyRpcFinish.count(), 0);
    TimeMachineForTest::getInstance()->processTimers(5000);
    QCOMPARE(spyRpcFinish.count(), 1);
    QCOMPARE(spyRpcFinish[0][1].toUuid(), id1);
    QCOMPARE(getReturnResult(spyRpcFinish[0]), "72");
}

QUuid test_rpc_simplified::invokeRpc(QString rpcName, QString paramName, QVariant paramValue)
{
    QVariantMap rpcParams;
    if(!paramName.isEmpty())
        rpcParams.insert(paramName, paramValue);
    QUuid id = m_rpcInvoker->invokeRPC(rpcName, rpcParams);
    TimeMachineObject::feedEventLoop();
    return id;
}

bool test_rpc_simplified::isRpcFound(QList<QVariant> spyArguments)
{
    return spyArguments.at(0).toBool();
}

QVariant test_rpc_simplified::getReturnResult(QList<QVariant> spyArguments)
{
    QVariantMap argMap = spyArguments[2].toMap();
    return argMap[VeinComponent::RemoteProcedureData::s_returnString];
}

QVariant test_rpc_simplified::getReturnError(QList<QVariant> spyArguments)
{
    QVariantMap argMap = spyArguments[2].toMap();
    return argMap[VeinComponent::RemoteProcedureData::s_errorMessageString];
}
