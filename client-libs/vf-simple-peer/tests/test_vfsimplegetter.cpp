#include "test_vfsimplegetter.h"
#include "veintestserver.h"
#include "vfcommandeventhandlersystem.h"
#include "vfsimpleentitysubscriber.h"
#include "vfsimplegetter.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;

QTEST_MAIN(test_vfsimplegetter)

void test_vfsimplegetter::checkErrorSignalFromUnsubscribedEntityInvalidComponent()
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
    getter->getComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toInt(), testEntityId);
    QCOMPARE(arguments.at(1).toString(), "foo");
    QCOMPARE(arguments.at(2).toBool(), false);
    QCOMPARE(arguments.at(3), QVariant());
}

void test_vfsimplegetter::checkGetFromUnsubscribedEntityValidComponent()
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
    getter->getComponent();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toInt(), systemEntityId);
    QCOMPARE(arguments.at(1).toString(), "EntityName");
    QCOMPARE(arguments.at(2).toBool(), true);
    QCOMPARE(arguments.at(3), QVariant("_System"));
}

void test_vfsimplegetter::checkGetFromUnsubscribedEntityValidComponentNetwork()
{

}

void test_vfsimplegetter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
