#include "test_test_vein_server.h"
#include "testdumpreporter.h"
#include <timemachineobject.h>
#include <QTest>

QTEST_MAIN(test_test_vein_server)

void test_test_vein_server::init()
{
    m_server = std::make_unique<TestVeinServer>();
    TimeMachineObject::feedEventLoop();
}

void test_test_vein_server::cleanup()
{
    m_server = nullptr;
}

void test_test_vein_server::unfinishedSession()
{
    m_server->resetEventSpyData();
    m_server->addTestEntities(1, 1);

    QFile fileEvents(":/vein-event-dumps/dumpUnfinishedSession.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpUnfinishedSession.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::completeSession()
{
    m_server->addTestEntities();
    m_server->resetEventSpyData();
    m_server->simulAllModulesLoaded("test_session1.json", QStringList() << "test_session1.json" << "test_session2.json");

    // ValueNew is missing on first two events: Why???
    QFile fileEvents(":/vein-event-dumps/dumpCompleteSessionSession.json");
    QVERIFY(fileEvents.open(QFile::ReadOnly));
    QByteArray jsonExpected = fileEvents.readAll();
    QByteArray jsonDumped = m_server->dumpEvents();
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));

    QFile fileStorage(":/vein-storage-dumps/dumpCompleteSessionSession.json");
    QVERIFY(fileStorage.open(QFile::ReadOnly));
    jsonExpected = fileStorage.readAll();
    jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}
