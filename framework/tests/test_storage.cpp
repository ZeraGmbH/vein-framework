#include "test_storage.h"
#include "testveinserverwithnet.h"
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
    serverNet.getServer()->simulAllModulesLoaded(QString("foo"), QStringList() << "fooList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/dumpInitial.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QString jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    if(jsonExpected != jsonDumped) {
        qWarning("Expected storage hash:");
        qInfo("%s", qPrintable(jsonExpected));
        qWarning("Dumped storage hash:");
        qInfo("%s", qPrintable(jsonDumped));
        QCOMPARE(jsonExpected, jsonDumped);
    }
}

static constexpr int testEntityId = 37;
static const char* entityName = "foo";
static const char* componentName = "bar";
static const char* componentValue = "value";

void test_storage::addEntity()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();
    serverNet.getServer()->simulAllModulesLoaded(QString("session"), QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/dumpEntityAdded.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QString jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    if(jsonExpected != jsonDumped) {
        qWarning("Expected storage hash:");
        qInfo("%s", qPrintable(jsonExpected));
        qWarning("Dumped storage hash:");
        qInfo("%s", qPrintable(jsonDumped));
        QCOMPARE(jsonExpected, jsonDumped);
    }
}
