#include "test_client_component_setter.h"
#include "vf_client_component_setter.h"
#include "veintestserver.h"
#include "vf_core_stack_client.h"
#include "vf_test_server_stack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_client_component_setter)

static constexpr int systemEntityId = 0;
static constexpr int invalidId = 1;
static constexpr int testId = 2;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VfCmdEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet() :
        server(&eventHandler)
    {
        eventHandler.addSubsystem(&cmdEventHandlerSystem);
    }
};

void test_client_component_setter::setInvalidIsEvil()
{
    ServerNoNet server;
    feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);

    setter->startSetComponent(QVariant(), QVariant());
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
}

void test_client_component_setter::setEqualEmitsOk()
{
    ServerNoNet server;
    feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);

    setter->startSetComponent("foo", "foo");
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
}

void test_client_component_setter::setToInvalidEntity()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(invalidId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    setter->startSetComponent("foo", "bar");
    feedEventLoop();

    // Nothing takes care!!!
    QCOMPARE(setterSpy.count(), 0);
}

void test_client_component_setter::setvalidEntityNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfCoreStackClient clientStack;
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverStack.eventHandler.addSubsystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    feedEventLoop();

    clientStack.subscribeEntity(systemEntityId);
    clientStack.subscribeEntity(testId);
    feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testId);
    clientStack.cmdEventHandlerSystem->addItem(entityItem);

    VfClientComponentSetterPtr setter = VfClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfClientComponentSetter::sigSetFinish);
    setter->startSetComponent(42, 0);
    feedEventLoop();

    QCOMPARE(setterSpy.count(), 1);
    QCOMPARE(setterSpy[0][0].toBool(), true);
}

void test_client_component_setter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}

