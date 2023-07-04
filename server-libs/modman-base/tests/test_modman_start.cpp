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
    VeinTestServer vfTestServer(&vfEventHandler);
    VfTestComponentAddListener vfListener;

    vfEventHandler.addSubsystem(&vfListener);

    feedEventLoop();

    VfTestComponentAddListener::AddHash hash = vfListener.getComponentHash();
    QCOMPARE(hash.size(), 1);
    int entityId = 0;
    QCOMPARE(hash[entityId].size(), 7);
    QCOMPARE(hash[entityId]["EntityName"], "_System");
    QCOMPARE(hash[entityId].contains("Session"), true);
    QCOMPARE(hash[entityId].contains("SessionsAvailable"), true);
    QCOMPARE(hash[entityId]["Entities"], QVariant());
    QCOMPARE(hash[entityId].contains("ModulesPaused"), true);
    QCOMPARE(hash[entityId].contains("Error_Messages"), true);
    QCOMPARE(hash[entityId].contains("LoggedComponents"), true);
}

void test_modman_start::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
