#include "test_atomic_client_component_getter.h"
#include "vfatomicclientcomponentgetter.h"
#include "veintestserver.h"
#include "vftestclientstack.h"
#include "vftestserverstack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_atomic_client_component_getter)

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;
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

void test_atomic_client_component_getter::errorSignalFromUnsubscribedEntityInvalidComponentNoNet()
{
    ServerNoNet server;
    feedEventLoop();
    QList<int> entities = server.server.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(testEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentGetterPtr getter = VfAtomicClientComponentGetter::create("foo", entityItem);
    entityItem->addItem(getter);
    QSignalSpy spy(getter.get(), &VfAtomicClientComponentGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_atomic_client_component_getter::getFromUnsubscribedEntityValidComponentNoNet()
{
    ServerNoNet server;
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    server.cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentGetterPtr getter = VfAtomicClientComponentGetter::create("EntityName", entityItem);
    entityItem->addItem(getter);
    QSignalSpy spy(getter.get(), &VfAtomicClientComponentGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_atomic_client_component_getter::noGetFromUnsubscribedEntityValidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentGetterPtr getter = VfAtomicClientComponentGetter::create("EntityName", entityItem);
    entityItem->addItem(getter);
    QSignalSpy getterSpy(getter.get(), &VfAtomicClientComponentGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(getterSpy.count(), 0);
}

void test_atomic_client_component_getter::getFromSubscribedEntityValidComponentNet()
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

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentGetterPtr getter = VfAtomicClientComponentGetter::create("EntityName", entityItem);
    entityItem->addItem(getter);
    QSignalSpy getterSpy(getter.get(), &VfAtomicClientComponentGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(getterSpy.count(), 1);
    QList<QVariant> arguments = getterSpy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_atomic_client_component_getter::getFromSubscribedEntityInvalidComponentNet()
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

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentGetterPtr getter = VfAtomicClientComponentGetter::create("foo", entityItem);
    entityItem->addItem(getter);
    QSignalSpy getterSpy(getter.get(), &VfAtomicClientComponentGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(getterSpy.count(), 1);
    QList<QVariant> arguments = getterSpy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_atomic_client_component_getter::getTwoDifferentComponent()
{
    // just server / no subscription
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server(&eventHandler);
    server.simulAllModulesLoaded("fooPath", QStringList() << "fooPath");
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();

    VfCmdEventItemEntityPtr entityItem = VfCmdEventItemEntity::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityItem);
    
    VfAtomicClientComponentGetterPtr getter1 = VfAtomicClientComponentGetter::create("EntityName", entityItem);
    entityItem->addItem(getter1);
    QSignalSpy getterSpy1(getter1.get(), &VfAtomicClientComponentGetter::sigGetFinish);
    
    VfAtomicClientComponentGetterPtr getter2 = VfAtomicClientComponentGetter::create("Session", entityItem);
    entityItem->addItem(getter2);
    QSignalSpy getterSpy2(getter2.get(), &VfAtomicClientComponentGetter::sigGetFinish);

    getter1->startGetComponent();
    getter2->startGetComponent();
    feedEventLoop();

    QCOMPARE(getterSpy1.count(), 1);
    QList<QVariant> arguments1 = getterSpy1[0];
    QCOMPARE(arguments1.at(0).toBool(), true);
    QCOMPARE(arguments1.at(1), QVariant("_System"));

    QCOMPARE(getterSpy2.count(), 1);
    QList<QVariant> arguments2 = getterSpy2[0];
    QCOMPARE(arguments2.at(0).toBool(), true);
    QCOMPARE(arguments2.at(1), QVariant("fooPath"));
}

void test_atomic_client_component_getter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
