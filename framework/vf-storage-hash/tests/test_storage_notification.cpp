#include "test_storage_notification.h"
#include "testveinserver.h"
#include "vs_storageeventsystem.h"
#include "vf_server_component_setter.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_storage_notification)

using namespace VeinStorage;

static constexpr int serverPort = 4242;

void test_storage_notification::getNullNotifierFromEmpty()
{
    StorageEventSystem hash;
    QCOMPARE(hash.getComponent(0, "foo"), nullptr);
    QCOMPARE(hash.getComponent(1, "foo"), nullptr);
    QCOMPARE(hash.getComponent(0, "bar"), nullptr);
    QCOMPARE(hash.getComponent(1, "bar"), nullptr);
}

void test_storage_notification::getNotifierForExisting()
{
    TestVeinServer server;
    server.simulAllModulesLoaded("session", QStringList() << "sessionList");
    AbstractEventSystem* storage = server.getStorage();

    QVERIFY(storage->getComponent(0, "EntityName"));
}

void test_storage_notification::getNotifierForNonExisting()
{
    TestVeinServer server;
    server.simulAllModulesLoaded("session", QStringList() << "sessionList");
    AbstractEventSystem* storage = server.getStorage();

    QCOMPARE(storage->getComponent(0, "foo"), nullptr);
}

void test_storage_notification::getNotifierForTwoExisting()
{
    TestVeinServer server;
    server.simulAllModulesLoaded("session", QStringList() << "sessionList");
    AbstractEventSystem* storage = server.getStorage();

    AbstractComponentPtr component1 = storage->getComponent(0, "EntityName");
    AbstractComponentPtr component2 = storage->getComponent(0, "Session");
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
    TestVeinServer server;
    server.addTestEntities(1, 1);
    server.simulAllModulesLoaded("session", QStringList() << "sessionList");

    AbstractEventSystem* storage = server.getStorage();
    AbstractComponentPtr component = storage->getComponent(testEntityId, componentName);
    QSignalSpy spy(component.get(), &AbstractComponent::sigValueChange);

    sendVeinSetAndProcess(&server, QVariant(), componentValue1); // valid old / change new
    sendVeinSetAndProcess(&server, componentValue2, componentValue2); // invalid old / change new
    sendVeinSetAndProcess(&server, componentValue2, componentValue1); // valid old / change new

    QCOMPARE(component->getValue(), componentValue1);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy[0][0], componentValue1);
    QCOMPARE(spy[1][0], componentValue2);
    QCOMPARE(spy[2][0], componentValue1);
}

void test_storage_notification::receiveNoChangeSignalOnSameValueByVein()
{
    TestVeinServer server;
    server.addTestEntities(1, 1);
    server.simulAllModulesLoaded("session", QStringList() << "sessionList");

    AbstractEventSystem* storage = server.getStorage();
    AbstractComponentPtr component = storage->getComponent(testEntityId, componentName);
    QSignalSpy spy(component.get(), &AbstractComponent::sigValueChange);

    sendVeinSetAndProcess(&server, QVariant(), componentValue1); // change new / valid old
    sendVeinSetAndProcess(&server, QVariant(), componentValue1); // same new / invalid old
    sendVeinSetAndProcess(&server, componentValue1, componentValue1); // same new / valid old

    QCOMPARE(component->getValue(), componentValue1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0], componentValue1);
}

void test_storage_notification::receiveOneSetSignalPerSetByVein()
{
    TestVeinServer server;
    server.addTestEntities(1, 1);
    server.simulAllModulesLoaded("session", QStringList() << "sessionList");

    AbstractEventSystem* storage = server.getStorage();
    AbstractComponentPtr component = storage->getComponent(testEntityId, componentName);
    QSignalSpy spy(component.get(), &AbstractComponent::sigValueSet);

    sendVeinSetAndProcess(&server, QVariant(), componentValue1); // change new / valid old
    sendVeinSetAndProcess(&server, componentValue1, componentValue1); // same new / valid old
    sendVeinSetAndProcess(&server, componentValue2, componentValue2); // change new / invalid old
    sendVeinSetAndProcess(&server, componentValue1, componentValue2); // same new / invalid old

    QCOMPARE(component->getValue(), componentValue2);
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy[0][0], componentValue1);
    QCOMPARE(spy[1][0], componentValue1);
    QCOMPARE(spy[2][0], componentValue2);
    QCOMPARE(spy[3][0], componentValue2);
}

void test_storage_notification::sendVeinSetAndProcess(TestVeinServer* server, QVariant oldValue, QVariant newValue)
{
    QEvent *event;
    event = VfServerComponentSetter::generateEvent(testEntityId, componentName, oldValue, newValue);
    server->sendEvent(event);
    TimeMachineObject::feedEventLoop();
}
