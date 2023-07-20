#include "test_vfsentityinsubscription.h"
#include "vfcommandeventhandlersystem.h"
#include "vfsentityinsubscription.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_vfsentityinsubscription)

void test_vfsentityinsubscription::intropectSystemEntitySignalReceived()
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer testServer(&eventHandler);
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
    eventHandler.addSubsystem(&cmdEventHandlerSystem);
    feedEventLoop();
    QList<int> entities = testServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);

    VfsEntityInSubscriptionPtr entityToSubscribe = VfsEntityInSubscription::create(0);
    cmdEventHandlerSystem.addItem(entityToSubscribe);
    QSignalSpy spy(entityToSubscribe.get(), &VfsEntityInSubscription::sigSubscribed);
    entityToSubscribe->sendSubscrption();
    feedEventLoop();

    QCOMPARE(spy.count(), 1);
}

void test_vfsentityinsubscription::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
