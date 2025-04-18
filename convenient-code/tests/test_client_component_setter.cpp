#include "test_client_component_setter.h"
#include "vf_client_component_setter.h"
#include "testveinserver.h"
#include "vf_core_stack_client.h"
#include "testveinserverwithmocknet.h"
#include "mocktcpnetworkfactory.h"
#include <timemachineobject.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_component_setter)

static constexpr int systemEntityId = 0;
static constexpr int invalidId = 1;
static constexpr int testId = 2;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    TestVeinServer server;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet()
    {
        server.appendEventSystem(&cmdEventHandlerSystem);
    }
};

void test_client_component_setter::setInvalidIsEvil()
{
    ServerNoNet server;
    TimeMachineObject::feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);

    setter->startSetComponent(QVariant(), QVariant());
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
}

void test_client_component_setter::setEqualEmitsOk()
{
    ServerNoNet server;
    TimeMachineObject::feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);

    setter->startSetComponent("foo", "foo");
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
}

void test_client_component_setter::setToInvalidEntity()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    TimeMachineObject::feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(invalidId);
    clientStack.addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    setter->startSetComponent("foo", "bar");
    TimeMachineObject::feedEventLoop();

    // Nothing takes care!!!
    QCOMPARE(setterSpy.count(), 0);
}

void test_client_component_setter::setvalidEntityNet()
{
    TestVeinServerWithMockNet serverNet(serverPort);

    VfCoreStackClient clientStack(VeinTcp::MockTcpNetworkFactory::create());
    clientStack.connectToServer("127.0.0.1", serverPort);
    TimeMachineObject::feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverNet.getServer()->appendEventSystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    TimeMachineObject::feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    clientStack.subscribeEntity(testId);
    TimeMachineObject::feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfEntityComponentEventItem::create(testId);
    clientStack.addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    setter->startSetComponent(42, 0);
    TimeMachineObject::feedEventLoop();

    QCOMPARE(setterSpy.count(), 1);
    QCOMPARE(setterSpy[0][0].toBool(), true);
}
