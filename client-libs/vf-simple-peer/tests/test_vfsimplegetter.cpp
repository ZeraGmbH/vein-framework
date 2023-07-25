#include "test_vfsimplegetter.h"
#include "veintestserver.h"
#include "vfsimpleentitysubscriber.h"
#include "vfsimplegetter.h"
#include "vftestclientstack.h"
#include "vftestserverstack.h"
#include "vtcp_workerfactorymethodstest.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;
static constexpr int serverPort = 4242;

QTEST_MAIN(test_vfsimplegetter)

void test_vfsimplegetter::errorSignalFromUnsubscribedEntityInvalidComponentNoNet()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfSimpleGetterPtr getter = VfSimpleGetter::create(testEntityId, "foo");
    cmdEventHandlerSystem.addItem(getter);
    QSignalSpy spy(getter.get(), &VfSimpleGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_vfsimplegetter::getFromUnsubscribedEntityValidComponentNoNet()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfSimpleGetterPtr getter = VfSimpleGetter::create(systemEntityId, "EntityName");
    cmdEventHandlerSystem.addItem(getter);
    QSignalSpy spy(getter.get(), &VfSimpleGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_vfsimplegetter::subsribeSystemEntity(VfCommandEventHandlerSystem* cmdEventHandlerSystem)
{
    VfSimpleEntitySubscriberPtr entityToSubscribe = VfSimpleEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem->addItem(entityToSubscribe);
    entityToSubscribe->sendSubscrption();
}

void test_vfsimplegetter::noGetFromUnsubscribedEntityValidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    VfSimpleGetterPtr getter = VfSimpleGetter::create(systemEntityId, "EntityName");
    cmdEventHandlerSystem.addItem(getter);
    QSignalSpy getterSpy(getter.get(), &VfSimpleGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(getterSpy.count(), 0);
}

void test_vfsimplegetter::getFromSubscribedEntityValidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    subsribeSystemEntity(&cmdEventHandlerSystem);
    feedEventLoop();

    VfSimpleGetterPtr getter = VfSimpleGetter::create(systemEntityId, "EntityName");
    cmdEventHandlerSystem.addItem(getter);
    QSignalSpy getterSpy(getter.get(), &VfSimpleGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(getterSpy.count(), 1);
    QList<QVariant> arguments = getterSpy[0];
    QCOMPARE(arguments.at(0).toBool(), true);
    QCOMPARE(arguments.at(1), QVariant("_System"));
}

void test_vfsimplegetter::getFromSubscribedEntityInvalidComponentNet()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VfTestServerStack serverStack(serverPort);

    VfTestClientStack clientStack;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    clientStack.eventHandler.addSubsystem(&cmdEventHandlerSystem);
    clientStack.tcpSystem.connectToServer("127.0.0.1", serverPort);
    feedEventLoop();

    subsribeSystemEntity(&cmdEventHandlerSystem);
    feedEventLoop();

    VfSimpleGetterPtr getter = VfSimpleGetter::create(systemEntityId, "foo");
    cmdEventHandlerSystem.addItem(getter);
    QSignalSpy getterSpy(getter.get(), &VfSimpleGetter::sigGetFinish);
    getter->startGetComponent();
    feedEventLoop();

    QCOMPARE(getterSpy.count(), 1);
    QList<QVariant> arguments = getterSpy[0];
    QCOMPARE(arguments.at(0).toBool(), false);
    QCOMPARE(arguments.at(1), QVariant());
}

void test_vfsimplegetter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
