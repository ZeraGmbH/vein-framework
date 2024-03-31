#include "test_storage.h"
#include "testveinserverwithnet.h"
#include "testdumpreporter.h"
#include "vtcp_workerfactorymethodstest.h"
#include <timemachineobject.h>
#include <QBuffer>
#include <QTest>

static constexpr int serverPort = 4242;

QTEST_MAIN(test_storage)

void test_storage::initTestCase()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
}

void test_storage::systemEntityOnly()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/dumpStorageInitial.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
}

static constexpr int testEntityId = 37;
static const char* entityName = "TestEntity";
static const char* componentName = "TestComponent";
static const char* componentValue = "TestInitialValue";

void test_storage::addEntity()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/dumpStorageEntityAdded.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::reportOnFail(jsonExpected, jsonDumped));
}
