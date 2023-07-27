#include "test_vfsimplesetter.h"
#include "vfsimpleentitysubscriber.h"
#include "vfsimplesetter.h"
#include "veintestserver.h"
#include "vftestclientstack.h"
#include "vftestserverstack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_vfsimplesetter)

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

void test_vfsimplesetter::setInvalidIsEvil()
{
    ServerNoNet server;
    feedEventLoop();

    VfSimpleSetterPtr setter = VfSimpleSetter::create(systemEntityId, "foo");
    server.cmdEventHandlerSystem.addItem(setter);

    setter->startSetComponent(QVariant(), QVariant());
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfSimpleSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
}

void test_vfsimplesetter::setEqualEmitsOk()
{
    ServerNoNet server;
    feedEventLoop();

    VfSimpleSetterPtr setter = VfSimpleSetter::create(systemEntityId, "foo");
    server.cmdEventHandlerSystem.addItem(setter);

    setter->startSetComponent("foo", "foo");
    // check event loop fired: connect after start
    QSignalSpy spy(setter.get(), &VfSimpleSetter::sigSetFinish);
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
}

void test_vfsimplesetter::setToInvalidEntity()
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

    VfSimpleSetterPtr setter = VfSimpleSetter::create(invalidId, "foo");
    cmdEventHandlerSystem.addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfSimpleSetter::sigSetFinish);
    setter->startSetComponent("foo", "bar");
    feedEventLoop();

    // Nothing takes care!!!
    QCOMPARE(setterSpy.count(), 0);
}

void test_vfsimplesetter::setvalidEntityNet()
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

    VfSimpleSetterPtr setter = VfSimpleSetter::create(testId, "foo");
    cmdEventHandlerSystem.addItem(setter);
    QSignalSpy setterSpy(setter.get(), &VfSimpleSetter::sigSetFinish);
    setter->startSetComponent(42, 0);
    feedEventLoop();

    QCOMPARE(setterSpy.count(), 1);
}

void test_vfsimplesetter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}

