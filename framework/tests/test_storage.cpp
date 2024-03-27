#include "test_storage.h"
#include "testveinserverwithnet.h"
#include "vf_core_stack_client.h"
#include "vtcp_workerfactorymethodstest.h"
#include <timemachineobject.h>
#include <QTest>

static constexpr int serverPort = 4242;
static constexpr int testEntityId = 37;
static const char* entityName = "foo";
static const char* componentName = "bar";
static const char* componentValue = "value";

QTEST_MAIN(test_storage)

void test_storage::serverSide()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestVeinServerWithNet serverNet(serverPort);

    VeinEvent::StorageSystem* storage = serverNet.getStorage();
    QVERIFY(storage->hasEntity(0)); // system entity
    QVERIFY(!storage->hasEntity(testEntityId));

    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();

    QVERIFY(storage->hasEntity(testEntityId));
    QVERIFY(storage->hasStoredValue(testEntityId, componentName));
    QCOMPARE(storage->getStoredValue(testEntityId, componentName), componentValue);
}

void test_storage::clientSide()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    TestVeinServerWithNet serverNet(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    TestVeinServer* server = serverNet.getServer();
    server->addEntity(testEntityId, entityName);
    server->addComponent(testEntityId, componentName, componentValue, false);
    TimeMachineObject::feedEventLoop();





    /*QVERIFY(storage->hasEntity(testEntityId));
    QVERIFY(storage->hasStoredValue(testEntityId, componentName));
    QCOMPARE(storage->getStoredValue(testEntityId, componentName), componentValue);*/
}
