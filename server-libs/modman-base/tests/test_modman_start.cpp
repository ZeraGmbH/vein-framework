#include "test_modman_start.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include "vftestcomponentaddlistener.h"
#include "vftestcomponentchangelistener.h"
#include <QAbstractEventDispatcher>
#include <QTest>

QTEST_MAIN(test_modman_start)

void test_modman_start::emptyModman()
{
    VeinEvent::EventHandler vfEventHandler;

    VfTestComponentAddListener vfAddListener;
    VfTestComponentChangeListener vfChangeListener;
    vfEventHandler.addSubsystem(&vfAddListener);
    vfEventHandler.addSubsystem(&vfChangeListener);

    VeinTestServer vfTestServer(&vfEventHandler);

    feedEventLoop();

    VfTestComponentAddListener::AddHash hash = vfAddListener.getComponentHash();
    QCOMPARE(hash.size(), 1);
    int entityId = 0;
    QCOMPARE(hash[entityId].size(), 7);
    QCOMPARE(hash[entityId]["EntityName"], "_System");
    QCOMPARE(hash[entityId].contains("Session"), true);
    QCOMPARE(hash[entityId].contains("SessionsAvailable"), true);
    QCOMPARE(hash[entityId]["Entities"], QVariant()); // _SYSTEM listed once ModuleManagerController::initializeEntity is called
    QCOMPARE(hash[entityId].contains("ModulesPaused"), true);
    QCOMPARE(hash[entityId].contains("Error_Messages"), true);
    QCOMPARE(hash[entityId].contains("LoggedComponents"), true);

    QCOMPARE(vfChangeListener.getComponentChangeList().size(), 0);
}

void test_modman_start::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
