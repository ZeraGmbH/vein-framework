#include "test_test_vein_server.h"
#include "testdumpreporter.h"
#include <QTest>

QTEST_MAIN(test_test_vein_server)

void test_test_vein_server::init()
{
    m_server = std::make_unique<TestVeinServer>();
}

void test_test_vein_server::cleanup()
{
    m_server = nullptr;
}

void test_test_vein_server::unfinishedSession()
{
    m_server->addTestEntities();

    QFile file(":/vein-storage-dumps/dumpUnfinishedSession.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_test_vein_server::completeSession()
{
    m_server->addTestEntities();
    m_server->simulAllModulesLoaded("test_session1.json", QStringList() << "test_session1.json" << "test_session2.json");

    QFile file(":/vein-storage-dumps/dumpCompleteSessionSession.json");
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped = m_server->dumpStorage(QList<int>() << 0 << m_server->getTestEntityComponentInfo().keys());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}
