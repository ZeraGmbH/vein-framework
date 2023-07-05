#include "test_modman_start.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include "vftestcomponentspy.h"
#include "vftestcomponentspyfilter.h"
#include <QAbstractEventDispatcher>
#include <QTest>

QTEST_MAIN(test_modman_start)

using VeinComponent::ComponentData;

void test_modman_start::emptyModman()
{
    VeinEvent::EventHandler vfEventHandler;

    VfTestComponentSpy vfAddListener(ComponentData::Command::CCMD_ADD);
    VfTestComponentSpy vfChangeListener(ComponentData::Command::CCMD_SET);
    vfEventHandler.addSubsystem(&vfAddListener);
    vfEventHandler.addSubsystem(&vfChangeListener);

    VeinTestServer vfTestServer(&vfEventHandler);
    feedEventLoop();

    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfAddListener.getComponentChangeList();
    int entityId = 0;
    QList<VfTestComponentSpy::TComponentInfo> entityComponents = VfTestComponentSpyFilter::filter(allComponents, entityId);
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

    QCOMPARE(vfChangeListener.getComponentChangeList().size(), 0);
}

void test_modman_start::modmanPlusOneEntity()
{
}

void test_modman_start::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
