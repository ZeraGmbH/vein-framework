#include "test_atomic_client_component_setter.h"
#include "vfatomicclientcomponentsetter.h"
#include "veintestserver.h"
#include "vftestclientstack.h"
#include "vftestserverstack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_atomic_client_component_setter)

static constexpr int systemEntityId = 0;
static constexpr int invalidId = 1;
static constexpr int testId = 2;
static constexpr int serverPort = 4242;

struct ServerNoNet
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    ServerNoNet() :
        server(&eventHandler)
    {
        eventHandler.addSubsystem(&cmdEventHandlerSystem);
    }
};

void test_atomic_client_component_setter::setInvalidIsEvil()
{
    ServerNoNet server;
    feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);

    VfAtomicClientComponentSetterPtr setter = VfAtomicClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);

    setter->startSetComponent(QVariant(), QVariant());
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfAtomicClientComponentSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
}

void test_atomic_client_component_setter::setEqualEmitsOk()
{
    ServerNoNet server;
    feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);

    VfAtomicClientComponentSetterPtr setter = VfAtomicClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);

    setter->startSetComponent("foo", "foo");
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfAtomicClientComponentSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
}

void test_atomic_client_component_setter::setToInvalidEntity()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    clientStack.subscribeEntityId(systemEntityId, &cmdEventHandlerSystem);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(invalidId);
    cmdEventHandlerSystem.addItem(entityItem);

    VfAtomicClientComponentSetterPtr setter = VfAtomicClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfAtomicClientComponentSetter::sigSetFinish);
    setter->startSetComponent("foo", "bar");
    feedEventLoop();

    // Nothing takes care!!!
    QCOMPARE(setterSpy.count(), 0);
}

void test_atomic_client_component_setter::setvalidEntityNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);
    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);

    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfCpp::VfCppEntity serverAdditionalEntity(testId);
    serverStack.eventHandler.addSubsystem(&serverAdditionalEntity);
    serverAdditionalEntity.initModule();
    serverAdditionalEntity.createComponent("foo", 42);
    feedEventLoop();

    clientStack.subscribeEntityId(systemEntityId, &cmdEventHandlerSystem);
    clientStack.subscribeEntityId(testId, &cmdEventHandlerSystem);
    feedEventLoop();
    
    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testId);
    cmdEventHandlerSystem.addItem(entityItem);

    VfAtomicClientComponentSetterPtr setter = VfAtomicClientComponentSetter::create("foo", entityItem);
    entityItem->addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfAtomicClientComponentSetter::sigSetFinish);
    setter->startSetComponent(42, 0);
    feedEventLoop();

    QCOMPARE(setterSpy.count(), 1);
    QCOMPARE(setterSpy[0][0].toBool(), true);
}

void test_atomic_client_component_setter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}

