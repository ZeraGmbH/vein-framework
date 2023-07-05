#include "test_modman_start.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include "vftestcomponentspyfilter.h"
#include "vf-cpp-entity.h"
#include <QAbstractEventDispatcher>
#include <QTest>

QTEST_MAIN(test_modman_start)

void test_modman_start::emptyModman()
{
    VeinEvent::EventHandler vfEventHandler;

    VeinTestServer vfTestServer(&vfEventHandler);
    feedEventLoop();

    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfTestServer.getComponentAddList();
    int systemEntityId = 0;
    QList<VfTestComponentSpy::TComponentInfo> entityComponents = VfTestComponentSpyFilter::filter(allComponents, systemEntityId);
    QCOMPARE(allComponents.size(), entityComponents.size());

    QCOMPARE(entityComponents.size(), 7);
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "EntityName"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents, "EntityName").newValue, "_System");
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "Session"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "SessionsAvailable"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "Entities"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents, "Entities").newValue, QVariant()); // _SYSTEM listed once ModuleManagerController::initializeEntity is called
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "ModulesPaused"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "Error_Messages"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "LoggedComponents"));

    QCOMPARE(vfTestServer.getComponentChangeList().size(), 0);
}

void test_modman_start::modmanPlusOneEntity()
{
}

void test_modman_start::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
