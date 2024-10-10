#include "test_storage_notification.h"
#include "testveinserverwithnet.h"
#include "vs_veinhash.h"
#include "vtcp_workerfactorymethodstest.h"
#include "vf_server_component_setter.h"
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

    VeinEvent::StorageComponentInterfacePtr component1 = storage->getComponent(0, "EntityName");
    VeinEvent::StorageComponentInterfacePtr component2 = storage->getComponent(0, "Session");
    QVERIFY(component1);
    QVERIFY(component2);
    QVERIFY(component1 != component2);
}

static constexpr int testEntityId = 10;
static const char* componentName = "ComponentName1";
static const char* componentValue1 = "TestValue1";
static const char* componentValue2 = "TestValue2";

//////////////////////////////////////////////////////////////////////////
// In the tests following we play around with old (invalid) values - it is
// unclear where they are used

void test_storage_notification::receiveOneChangeSignalPerChangeByVein()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addTestEntities(1, 1);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    VeinEvent::StorageComponentInterfacePtr component = storage->getComponent(testEntityId, componentName);
    QSignalSpy spy(component.get(), &VeinEvent::StorageComponentInterface::sigValueChange);

    sendVeinSetAndProcess(server, QVariant(), componentValue1); // valid old / change new
    sendVeinSetAndProcess(server, componentValue2, componentValue2); // invalid old / change new
    sendVeinSetAndProcess(server, componentValue2, componentValue1); // valid old / change new

    QCOMPARE(component->getValue(), componentValue1);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy[0][0], componentValue1);
    QCOMPARE(spy[1][0], componentValue2);
    QCOMPARE(spy[2][0], componentValue1);
}

void test_storage_notification::receiveNoChangeSignalOnSameValueByVein()
{
    TestVeinServerWithNet serverNet(serverPort);
    TestVeinServer* server = serverNet.getServer();
    server->addTestEntities(1, 1);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    VeinEvent::StorageComponentInterfacePtr component = storage->getComponent(testEntityId, componentName);
    QSignalSpy spy(component.get(), &VeinEvent::StorageComponentInterface::sigValueChange);

    sendVeinSetAndProcess(server, QVariant(), componentValue1); // change new / valid old
    sendVeinSetAndProcess(server, QVariant(), componentValue1); // same new / invalid old
    sendVeinSetAndProcess(server, componentValue1, componentValue1); // same new / valid old

    QCOMPARE(component->getValue(), componentValue1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], componentValue1);
}

void test_storage_notification::sendVeinSetAndProcess(TestVeinServer* server, QVariant oldValue, QVariant newValue)
{
    QEvent *event;
    event = VfServerComponentSetter::generateEvent(testEntityId, componentName, oldValue, newValue);
    server->sendEvent(event);
    TimeMachineObject::feedEventLoop();
}
