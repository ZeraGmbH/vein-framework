#include "test_atomic_client_entity_subscriber.h"
#include "vfatomiccliententitysubscriber.h"
#include "vfcommandeventhandlersystem.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_atomic_client_entity_subscriber)

static constexpr int systemEntityId = 0;
static constexpr int noneExistentEnitityId = 42;

void test_atomic_client_entity_subscriber::intropectSystemEntitySignalReceived()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);
    
    VfAtomicClientEntitySubscriberPtr entityToSubscribe = VfAtomicClientEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfAtomicClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
}

void test_atomic_client_entity_subscriber::trySubscribeOnNonExistantEntity()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    
    VfAtomicClientEntitySubscriberPtr entityToSubscribe = VfAtomicClientEntitySubscriber::create(noneExistentEnitityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfAtomicClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
}

void test_atomic_client_entity_subscriber::introspectComponentNames()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    
    VfAtomicClientEntitySubscriberPtr entityToSubscribe = VfAtomicClientEntitySubscriber::create(systemEntityId);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfAtomicClientEntitySubscriber::sigSubscribed);
    entityToSubscribe->sendSubscription();
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

void test_atomic_client_entity_subscriber::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
