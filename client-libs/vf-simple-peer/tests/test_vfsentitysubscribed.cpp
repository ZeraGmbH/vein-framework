#include "test_vfsentitysubscribed.h"
#include "veintestserver.h"
#include "vfcommandeventhandlersystem.h"
#include "vfsentityinsubscription.h"
#include "vfsentitysubscribed.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

static constexpr int systemEntityId = 0;
static constexpr int testEntityId = 1;

QTEST_MAIN(test_vfsentitysubscribed)

void test_vfsentitysubscribed::checkErrorSignalFromUnsubscribedEntityInvalidComponent()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfsEntitySubscribedPtr unsubscribedEntity = VfsEntitySubscribed::create(testEntityId, QStringList() << "foo");
    cmdEventHandlerSystem.addItem(unsubscribedEntity);
    QSignalSpy spy(unsubscribedEntity.get(), &VfsEntitySubscribed::sigGetFinish);
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

    VfsEntitySubscribedPtr unsubscribedEntity = VfsEntitySubscribed::create(systemEntityId, QStringList() << "EntityName");
    cmdEventHandlerSystem.addItem(unsubscribedEntity);
    QSignalSpy spy(unsubscribedEntity.get(), &VfsEntitySubscribed::sigGetFinish);
    unsubscribedEntity->getComponent("EntityName");
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toInt(), systemEntityId);
    QCOMPARE(arguments.at(1).toString(), "EntityName");
    QCOMPARE(arguments.at(2).toBool(), false);
    QCOMPARE(arguments.at(3), QVariant());
}*/

void test_vfsentitysubscribed::checkOkSignalFromSubscribedEntityValidComponent()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfsEntityInSubscriptionPtr entityToSubscribe = VfsEntityInSubscription::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    entityToSubscribe->sendSubscrption();
    feedEventLoop();
    QStringList componentNames = entityToSubscribe->getComponentNames();

    VfsEntitySubscribedPtr subscribedEntity = VfsEntitySubscribed::create(systemEntityId, componentNames);
    cmdEventHandlerSystem.addItem(subscribedEntity);
    QSignalSpy spy(subscribedEntity.get(), &VfsEntitySubscribed::sigGetFinish);
    subscribedEntity->getComponent("EntityName");
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy[0];
    QCOMPARE(arguments.at(0).toInt(), systemEntityId);
    QCOMPARE(arguments.at(1).toString(), "EntityName");
    QCOMPARE(arguments.at(2).toBool(), true);
}

void test_vfsentitysubscribed::getValidComponent()
{

}

void test_vfsentitysubscribed::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
