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
    QFile file(":/vein-storage-dumps/dumpStorageInitial.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
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
    TimeMachineObject::feedEventLoop();
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/vein-storage-dumps/dumpStorageEntityAdded.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_storage::addEntityAndComponent()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/vein-storage-dumps/dumpStorageEntityComponentAdd.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_storage::addRemoveEntity()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

    VeinComponent::EntityData *eData = new VeinComponent::EntityData();
    eData->setEntityId(testEntityId);
    eData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    eData->setCommand(VeinComponent::EntityData::Command::ECMD_REMOVE);
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
    server->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/vein-storage-dumps/dumpStorageEntityAddRemove.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_storage::addRemoveComponent()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(testEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_REMOVE);
    cData->setComponentName(componentName);
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    server->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/vein-storage-dumps/dumpStorageComponentAddRemove.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}

void test_storage::setComponent()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(testEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    cData->setNewValue("SetTestValue");
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    server->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QFile file(":/vein-storage-dumps/dumpStorageComponentSet.json");
    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray jsonExpected = file.readAll();
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    storage->dumpToFile(&buff, QList<int>());
    QVERIFY(TestDumpReporter::compareAndLogOnDiff(jsonExpected, jsonDumped));
}
