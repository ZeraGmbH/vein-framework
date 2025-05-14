#include "test_test_vein_server.h"
#include "mocklxdmsessionchangeparamgenerator.h"
#include "testloghelpers.h"
#include "vf_entity_rpc_event_handler.h"
#include <QSignalSpy>
#include <timemachineobject.h>
#include <QTest>

QTEST_MAIN(test_test_vein_server)

void test_test_vein_server::cleanup()
{
    m_server = nullptr;
}

void test_test_vein_server::unfinishedSession()
{
    startServer(false);
    m_server->resetEventSpyData();
    m_server->addTestEntities(1, 1);

    QFile fileEvents(":/vein-event-dumps/dumpUnfinishedSession.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpUnfinishedSession.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::completeSession()
{
    startServer(false);
    m_server->addTestEntities();
    m_server->resetEventSpyData();
    m_server->simulAllModulesLoaded("test_session1.json", QStringList() << "test_session1.json" << "test_session2.json");

    QFile fileEvents(":/vein-event-dumps/dumpCompleteSessionSession.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpCompleteSessionSession.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::changeSession()
{
    startServer(false);
    m_server->addTestEntities();
    m_server->simulAllModulesLoaded("test_session1.json", QStringList() << "test_session1.json" << "test_session2.json");
    m_server->resetEventSpyData();

    m_server->changeSession("test_session2.json");

    QFile fileEvents(":/vein-event-dumps/dumpChangeSession.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpChangeSession.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::completeSessionWithValidXSession()
{
    startServer(false);
    m_server->addTestEntities();
    m_server->resetEventSpyData();
    m_server->simulAllModulesLoaded("test_session1.json", QStringList() << "test_session1.json" << "test_session2.json");

    QFile fileEvents(":/vein-event-dumps/dumpCompleteSessionWithLxdm.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpCompleteSessionWithLxdm.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::completeSessionXSessionChangePass()
{
    startServer(true);
    m_server->addTestEntities();
    m_server->simulAllModulesLoaded("test_session1.json", QStringList() << "test_session1.json" << "test_session2.json");
    m_server->resetEventSpyData();

    m_server->setComponentClientTransaction(0, "XSession", "Foo");

    QFile fileEvents(":/vein-event-dumps/dumpChangeXSession.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpChangeXSession.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::completeSessionXSessionChangeFail()
{
    startServer(false);
    m_server->addTestEntities();
    m_server->simulAllModulesLoaded("test_session1.json", QStringList() << "test_session1.json" << "test_session2.json");
    m_server->resetEventSpyData();

    m_server->setComponentClientTransaction(0, "XSession", "Foo");

    QFile fileEvents(":/vein-event-dumps/dumpChangeXSessionFail.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpChangeXSessionFail.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestLogHelpers::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::invokeRpc()
{
    startServer(false);

    vfEntityRpcEventHandler rpcEventHandler;
    m_server->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy invokerSpy(m_server.get(), &TestVeinServer::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    QUuid id = m_server->clientInvokeRpc(rpcEventHandler.getVeinEntity()->getEntityId(), "RPC_forTest", parameters);

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), true);
    QCOMPARE(arguments.at(1), id);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, false);
}

void test_test_vein_server::invokeInvalidRpc()
{
    startServer(false);

    vfEntityRpcEventHandler rpcEventHandler;
    m_server->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy invokerSpy(m_server.get(), &TestVeinServer::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    QUuid id = m_server->clientInvokeRpc(rpcEventHandler.getVeinEntity()->getEntityId(), "foo", parameters);

    QCOMPARE(invokerSpy.count(), 1);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), false);
    QCOMPARE(arguments.at(1), id);
    QVariantMap argMap = arguments[2].toMap();
    QVariant resultData = argMap[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, QVariant());
}

void test_test_vein_server::invokeRpcTwice()
{
    startServer(false);

    vfEntityRpcEventHandler rpcEventHandler;
    m_server->appendEventSystem(rpcEventHandler.getVeinEntity());
    rpcEventHandler.initOnce();
    TimeMachineObject::feedEventLoop();

    QSignalSpy invokerSpy(m_server.get(), &TestVeinServer::sigRPCFinished);
    QVariantMap parameters;
    parameters["p_param"] = true;
    QUuid id1 = m_server->clientInvokeRpc(rpcEventHandler.getVeinEntity()->getEntityId(), "RPC_forTest", parameters);
    parameters["p_param"] = false;
    QUuid id2 = m_server->clientInvokeRpc(rpcEventHandler.getVeinEntity()->getEntityId(), "RPC_forTest", parameters);

    QCOMPARE(invokerSpy.count(), 2);
    QList<QVariant> arguments = invokerSpy[0];
    QCOMPARE(arguments.at(0), true);
    QCOMPARE(arguments.at(1), id1);
    QVariant resultData = arguments[2].toMap()[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, false);

    arguments = invokerSpy[1];
    QCOMPARE(arguments.at(0), true);
    QCOMPARE(arguments.at(1), id2);
    resultData = arguments[2].toMap()[VeinComponent::RemoteProcedureData::s_returnString];
    QCOMPARE(resultData, true);
}

void test_test_vein_server::startServer(bool restartServicePasses)
{
    m_server = std::make_unique<TestVeinServer>(MockLxdmSessionChangeParamGenerator::generateTestSessionChanger(restartServicePasses));
    TimeMachineObject::feedEventLoop();
}
