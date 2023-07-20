#include "test_vfsentityinsubscription.h"
#include "vfcommandeventhandlersystem.h"
#include "vfsentityinsubscription.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_vfsentityinsubscription)

static constexpr int systemEnitityId = 0;
static constexpr int noneExistentEnitityId = 42;

void test_vfsentityinsubscription::intropectSystemEntitySignalReceived()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfsEntityInSubscriptionPtr entityToSubscribe = VfsEntityInSubscription::create(systemEnitityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfsEntityInSubscription::sigSubscribed);
    entityToSubscribe->sendSubscrption();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
}

void test_vfsentityinsubscription::trySubscribeOnNonExistantEntity()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();

    VfsEntityInSubscriptionPtr entityToSubscribe = VfsEntityInSubscription::create(noneExistentEnitityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfsEntityInSubscription::sigSubscribed);
    entityToSubscribe->sendSubscrption();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
}

void test_vfsentityinsubscription::introspectComponentNames()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();

    VfsEntityInSubscriptionPtr entityToSubscribe = VfsEntityInSubscription::create(systemEnitityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfsEntityInSubscription::sigSubscribed);
    entityToSubscribe->sendSubscrption();
    feedEventLoop();

    QStringList componentNames = entityToSubscribe->getComponentNames();
    QCOMPARE(componentNames.size(), 7);
    QVERIFY(componentNames.contains("EntityName"));
    QVERIFY(componentNames.contains("Session"));
    QVERIFY(componentNames.contains("SessionsAvailable"));
    QVERIFY(componentNames.contains("Entities"));
    QVERIFY(componentNames.contains("ModulesPaused"));
    QVERIFY(componentNames.contains("Error_Messages"));
    QVERIFY(componentNames.contains("LoggedComponents"));

}

void test_vfsentityinsubscription::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
