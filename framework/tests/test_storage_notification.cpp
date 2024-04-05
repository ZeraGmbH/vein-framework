#include "test_storage_notification.h"
#include "testveinserverwithnet.h"
#include "vs_veinhash.h"
#include "vtcp_workerfactorymethodstest.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_storage_notification)

static constexpr int serverPort = 4242;

void test_storage_notification::initTestCase()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
}

void test_storage_notification::getNullNotifierFromEmpty()
{
    VeinStorage::VeinHash hash;
    QCOMPARE(hash.getComponent(0, "foo"), nullptr);
    QCOMPARE(hash.getComponent(1, "foo"), nullptr);
    QCOMPARE(hash.getComponent(0, "bar"), nullptr);
    QCOMPARE(hash.getComponent(1, "bar"), nullptr);
}

void test_storage_notification::getNotifierForExisting()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");
    VeinEvent::StorageSystem* storage = serverNet.getStorage();

    QVERIFY(storage->getComponent(0, "EntityName"));
}

void test_storage_notification::getNotifierForNonExisting()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");
    VeinEvent::StorageSystem* storage = serverNet.getStorage();

    QCOMPARE(storage->getComponent(0, "foo"), nullptr);
}

void test_storage_notification::getNotifierForTwoExisting()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");
    VeinEvent::StorageSystem* storage = serverNet.getStorage();

    VeinEvent::StorageComponentInterface *component1 = storage->getComponent(0, "EntityName");
    VeinEvent::StorageComponentInterface *component2 = storage->getComponent(0, "Session");
    QVERIFY(component1);
    QVERIFY(component2);
    QVERIFY(component1 != component2);
}

static constexpr int testEntityId = 37;
static const char* entityName = "TestEntity";
static const char* componentName = "TestComponent";
static const char* componentValue = "TestInitialValue";

void test_storage_notification::receiveOneSignalOnChangeByVein()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    VeinEvent::StorageComponentInterface *component = storage->getComponent(testEntityId, componentName);

    QSignalSpy spy(component, &VeinEvent::StorageComponentInterface::sigValueChange);
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

    QCOMPARE(component->getValue(), "SetTestValue");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], "SetTestValue");
}

void test_storage_notification::receiveNoSignalOnSameValueByVein()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    VeinEvent::StorageComponentInterface *component = storage->getComponent(testEntityId, componentName);

    QSignalSpy spy(component, &VeinEvent::StorageComponentInterface::sigValueChange);
    VeinComponent::ComponentData *cData = new VeinComponent::ComponentData();
    cData->setEntityId(testEntityId);
    cData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    cData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    cData->setCommand(VeinComponent::ComponentData::Command::CCMD_SET);
    cData->setComponentName(componentName);
    cData->setNewValue(componentValue);
    VeinEvent::CommandEvent *event = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, cData);
    server->sendEvent(event);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(component->getValue(), componentValue);
    QCOMPARE(spy.count(), 0);
}

