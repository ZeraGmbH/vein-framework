#include "test_rpc_simplified.h"
#include "mocktcpnetworkfactory.h"
#include <timerfactoryqtfortest.h>
#include <timemachinefortest.h>
#include <testloghelpers.h>
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
    m_veinEventDump = QJsonObject();
    setupSpy(m_veinEventDump);
    m_clientStack->subscribeEntity(entityIdWithRpc);

    TimeMachineObject::feedEventLoop();

    m_rpcInvoker = std::make_shared<VfRPCInvoker>(entityIdWithRpc, std::make_unique<VfClientRPCInvoker>());
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
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "p_param", 72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QVERIFY(isRpcFound(spyRpcFinish[0]));
    QCOMPARE(getReturnResult(spyRpcFinish[0]), "72");

    QFile file(":/vein-event-dumps/dumpCallRpcValidParam.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(m_veinEventDump);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_rpc_simplified::callRpcInvalidParamValue()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "p_param", -72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QVERIFY(isRpcFound(spyRpcFinish[0]));
    QCOMPARE(getReturnError(spyRpcFinish[0]), "Error");

    QFile file(":/vein-event-dumps/dumpCallRpcInvalidParamValue.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(m_veinEventDump);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_rpc_simplified::callRpcInvalidParamName()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "foo", 72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QVERIFY(!isRpcFound(spyRpcFinish[0]));

    QFile file(":/vein-event-dumps/dumpCallRpcInvalidParamName.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(m_veinEventDump);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_rpc_simplified::callRpcMissingParam()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "", "");
    QCOMPARE(spyRpcFinish.count(), 1);

    QList<QVariant> arguments = spyRpcFinish[0];
    QVERIFY(!isRpcFound(spyRpcFinish[0]));

    QFile file(":/vein-event-dumps/dumpCallRpcMissingParam.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(m_veinEventDump);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_rpc_simplified::callInvalidRpc()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished);
    invokeRpc("foo", "p_param", -72);
    QCOMPARE(spyRpcFinish.count(), 1);

    QVERIFY(!isRpcFound(spyRpcFinish[0]));

    QFile file(":/vein-event-dumps/dumpCallInvalidRpc.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(m_veinEventDump);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_rpc_simplified::callRPCTwice()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished);
    invokeRpc("RPC_forTest", "p_param", 72);
    invokeRpc("RPC_forTest", "p_param", 48);
    QCOMPARE(spyRpcFinish.count(), 2);

    QCOMPARE(getReturnResult(spyRpcFinish[0]), "72");
    QCOMPARE(getReturnResult(spyRpcFinish[1]), "48");

    QFile file(":/vein-event-dumps/dumpCallRPCTwice.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(m_veinEventDump);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_rpc_simplified::callRPCRespondingAfterDelay()
{
    QSignalSpy spyRpcFinish(m_rpcInvoker.get(), &VfRPCInvoker::sigRPCFinished);
    QVariantMap rpcParams;
    rpcParams.insert("p_param", 72);
    rpcParams.insert("p_delayMs", 5000);
    m_rpcInvoker->invokeRPC("RPC_addDelay", rpcParams);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spyRpcFinish.count(), 0);
    TimeMachineForTest::getInstance()->processTimers(5000);
    QCOMPARE(spyRpcFinish.count(), 1);
    QCOMPARE(getReturnResult(spyRpcFinish[0]), "72");

    QFile file(":/vein-event-dumps/dumpcallRPCRespondingAfterDelay.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = TestLogHelpers::dump(m_veinEventDump);
    QVERIFY(TestLogHelpers::compareAndLogOnDiffJson(jsonExpected, jsonDumped));
}

void test_rpc_simplified::invokeRpc(QString rpcName, QString paramName, QVariant paramValue)
{
    QVariantMap rpcParams;
    if(!paramName.isEmpty())
        rpcParams.insert(paramName, paramValue);
    m_rpcInvoker->invokeRPC(rpcName, rpcParams);
    TimeMachineObject::feedEventLoop();
}

bool test_rpc_simplified::isRpcFound(QList<QVariant> spyArguments)
{
    return spyArguments.at(0).toBool();
}

QVariant test_rpc_simplified::getReturnResult(QList<QVariant> spyArguments)
{
    QVariantMap argMap = spyArguments[1].toMap();
    return argMap[VeinComponent::RemoteProcedureData::s_returnString];
}

QVariant test_rpc_simplified::getReturnError(QList<QVariant> spyArguments)
{
    QVariantMap argMap = spyArguments[1].toMap();
    return argMap[VeinComponent::RemoteProcedureData::s_errorMessageString];
}

void test_rpc_simplified::setupSpy(QJsonObject &jsonEvents)
{
    m_serverCmdEventSpyTop = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "server-enter");
    m_serverNet->getServer()->prependEventSystem(m_serverCmdEventSpyTop.get());

    m_serverCmdEventSpyBottom = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "server-fallthrough");
    connect(m_serverNet->getServer()->getEventHandler(), &VeinEvent::EventHandler::sigEventAccepted,
            m_serverCmdEventSpyBottom.get(), &TestJsonSpyEventSystem::onEventAccepted);
    m_serverNet->getServer()->appendEventSystem(m_serverCmdEventSpyBottom.get());

    m_clientCmdEventSpyTop = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "client-enter");
    m_clientStack->prependEventSystem(m_clientCmdEventSpyTop.get());

    m_clientCmdEventSpyBottom = std::make_unique<TestJsonSpyEventSystem>(&jsonEvents, "client-fallthrough");
    connect(m_clientStack->getEventHandler(), &VeinEvent::EventHandler::sigEventAccepted,
            m_clientCmdEventSpyBottom.get(), &TestJsonSpyEventSystem::onEventAccepted);
    m_clientStack->appendEventSystem(m_clientCmdEventSpyBottom.get());
}
