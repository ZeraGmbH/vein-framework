#include "test_vfsimplegetter.h"
#include "vfsimplegetter.h"
#include "veintestserver.h"
#include "vfsimpleentitysubscriber.h"
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

void test_vfsimplegetter::getTwoDifferentComponent()
{
    // just server / no subscription
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server(&eventHandler);
    server.simulAllModulesLoaded("fooPath", QStringList() << "fooPath");
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();

    VfSimpleGetterPtr getter1 = VfSimpleGetter::create(systemEntityId, "EntityName");
    cmdEventHandlerSystem.addItem(getter1);
    QSignalSpy getterSpy1(getter1.get(), &VfSimpleGetter::sigGetFinish);

    VfSimpleGetterPtr getter2 = VfSimpleGetter::create(systemEntityId, "Session");
    cmdEventHandlerSystem.addItem(getter2);
    QSignalSpy getterSpy2(getter2.get(), &VfSimpleGetter::sigGetFinish);

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

void test_vfsimplegetter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
