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

    VfSimpleGetterPtr unsubscribedEntity = VfSimpleGetter::create(testEntityId, QStringList() << "foo");
    cmdEventHandlerSystem.addItem(unsubscribedEntity);
    QSignalSpy spy(unsubscribedEntity.get(), &VfSimpleGetter::sigGetFinish);
    unsubscribedEntity->getComponent("foo");
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toInt(), testEntityId);
    QCOMPARE(arguments.at(1).toString(), "foo");
    QCOMPARE(arguments.at(2).toBool(), false);
    QCOMPARE(arguments.at(3), QVariant());
}

/*void test_vfsentitysubscribed::checkErrorSignalFromUnsubscribedEntityValidComponent()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfSimpleGetterPtr unsubscribedEntity = VfSimpleGetter::create(systemEntityId, QStringList() << "EntityName");
    cmdEventHandlerSystem.addItem(unsubscribedEntity);
    QSignalSpy spy(unsubscribedEntity.get(), &VfSimpleGetter::sigGetFinish);
    unsubscribedEntity->getComponent("EntityName");
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toInt(), systemEntityId);
    QCOMPARE(arguments.at(1).toString(), "EntityName");
    QCOMPARE(arguments.at(2).toBool(), false);
    QCOMPARE(arguments.at(3), QVariant());
}*/

void test_vfsimplegetter::checkOkSignalFromSubscribedEntityValidComponent()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfSimpleEntitySubscriberPtr entityToSubscribe = VfSimpleEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    entityToSubscribe->sendSubscrption();
    feedEventLoop();
    QStringList componentNames = entityToSubscribe->getComponentNames();

    VfSimpleGetterPtr subscribedEntity = VfSimpleGetter::create(systemEntityId, componentNames);
    cmdEventHandlerSystem.addItem(subscribedEntity);
    QSignalSpy spy(subscribedEntity.get(), &VfSimpleGetter::sigGetFinish);
    subscribedEntity->getComponent("EntityName");
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toInt(), systemEntityId);
    QCOMPARE(arguments.at(1).toString(), "EntityName");
    QCOMPARE(arguments.at(2).toBool(), true);
}

void test_vfsimplegetter::getValidComponent()
{

}

void test_vfsimplegetter::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
