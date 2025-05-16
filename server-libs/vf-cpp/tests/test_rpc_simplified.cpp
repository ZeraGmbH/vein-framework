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
    QVariantMap rpcParams;
    rpcParams.insert("p_param", 72);
    m_rpcInvoker->invokeRPC("RPC_forTest", rpcParams);
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, "72");
}

void test_rpc_simplified::callRpcInvalidParamValue()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", -72);
    m_rpcInvoker->invokeRPC("RPC_forTest", rpcParams);
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), true);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_errorMessageString];
    QCOMPARE(resultData, "Error");
}

void test_rpc_simplified::callRpcInvalidParamName()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("foo", 72);
    m_rpcInvoker->invokeRPC("RPC_forTest", rpcParams);
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}

void test_rpc_simplified::callRpcMissingParam()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap rpcParams;
    m_rpcInvoker->invokeRPC("RPC_forTest", rpcParams);
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}

void test_rpc_simplified::callInvalidRpc()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", -72);
    m_rpcInvoker->invokeRPC("foo", rpcParams);
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QCOMPARE(arguments.at(0), false);
}

void test_rpc_simplified::callRPCTwice()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfClientRPCInvoker::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", 72);
    QUuid id1 = m_rpcInvoker->invokeRPC("RPC_forTest", rpcParams);
    TimeMachineObject::feedEventLoop();
    rpcParams.insert("p_param", 48);
    QUuid id2 = m_rpcInvoker->invokeRPC("RPC_forTest", rpcParams);
    TimeMachineObject::feedEventLoop();
    QCOMPARE(spyRpcFinish.count(), 2);

    QCOMPARE(spyRpcFinish[0][1].toUuid(), id1);
    QCOMPARE(spyRpcFinish[0][2].toMap()[VeinComponent::RemoteProcedureData::s_returnString], "72");
    QCOMPARE(spyRpcFinish[1][1].toUuid(), id2);
    QCOMPARE(spyRpcFinish[1][2].toMap()[VeinComponent::RemoteProcedureData::s_returnString], "48");
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
    QCOMPARE(spyRpcFinish[0][2].toMap()[VeinComponent::RemoteProcedureData::s_returnString], "72");
}
