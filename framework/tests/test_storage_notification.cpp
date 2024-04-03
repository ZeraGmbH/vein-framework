#include "test_storage_notification.h"
#include "testveinserverwithnet.h"
#include "vs_veinhash.h"
#include "vtcp_workerfactorymethodstest.h"
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
    QCOMPARE(hash.getChangeSignalizer(0, "foo"), nullptr);
    QCOMPARE(hash.getChangeSignalizer(1, "foo"), nullptr);
    QCOMPARE(hash.getChangeSignalizer(0, "bar"), nullptr);
    QCOMPARE(hash.getChangeSignalizer(1, "bar"), nullptr);
}

void test_storage_notification::getNotifierForExisting()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");
    VeinEvent::StorageSystem* storage = serverNet.getStorage();

    QVERIFY(storage->getChangeSignalizer(0, "EntityName"));
}

void test_storage_notification::getNotifierForNonExisting()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");
    VeinEvent::StorageSystem* storage = serverNet.getStorage();

    QCOMPARE(storage->getChangeSignalizer(0, "foo"), nullptr);
}

void test_storage_notification::getNotifierForExistingTwice()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");
    VeinEvent::StorageSystem* storage = serverNet.getStorage();

    VeinEvent::ComponentChangeSignalizer *notif1 = storage->getChangeSignalizer(0, "EntityName");
    VeinEvent::ComponentChangeSignalizer *notif2 = storage->getChangeSignalizer(0, "EntityName");

    QCOMPARE(notif1, notif2);
}

void test_storage_notification::getNotifierForTwoExisting()
{
    TestVeinServerWithNet serverNet(serverPort);
    serverNet.getServer()->simulAllModulesLoaded("session", QStringList() << "sessionList");
    VeinEvent::StorageSystem* storage = serverNet.getStorage();

    VeinEvent::ComponentChangeSignalizer *notif1 = storage->getChangeSignalizer(0, "EntityName");
    VeinEvent::ComponentChangeSignalizer *notif2 = storage->getChangeSignalizer(0, "Session");
    QVERIFY(notif1);
    QVERIFY(notif2);
    QVERIFY(notif1 != notif2);
}

