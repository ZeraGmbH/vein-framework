#include "test_modman_start.h"
#include "modulemanagercontroller.h"
#include "ve_eventhandler.h"
#include "vf-cpp-entity.h"
#include "vftestcomponentaddlistener.h"
#include "vftestcomponentchangelistener.h"
#include "vn_introspectionsystem.h"
#include "vs_veinhash.h"
#include <QAbstractEventDispatcher>
#include <QTest>

QTEST_MAIN(test_modman_start)

void test_modman_start::emptyModman()
{
    VeinEvent::EventHandler vfEventHandler;
    ModuleManagerController mmController;
    VeinNet::IntrospectionSystem introspectionSystem;
    VeinStorage::VeinHash storageSystem;
    VfTestComponentAddListener vfListener;

    mmController.setStorage(&storageSystem);

    QList<VeinEvent::EventSystem*> subSystems;
    // do not reorder??
    subSystems.append(&mmController);
    subSystems.append(&introspectionSystem);
    subSystems.append(&storageSystem);
    subSystems.append(&vfListener);
    vfEventHandler.setSubsystems(subSystems);

    mmController.initOnce();

    feedEventLoop();

    VfTestComponentAddListener::AddHash hash = vfListener.getComponentHash();
    QCOMPARE(hash.size(), 1);
    int entityId = 0;
    QCOMPARE(hash[entityId].size(), 8);
    QCOMPARE(hash[entityId]["EntityName"], "_System");
    QCOMPARE(hash[entityId].contains("ServerAddressList"), true);
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
