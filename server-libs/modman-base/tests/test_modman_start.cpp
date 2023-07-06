#include "test_modman_start.h"
#include "veintestserver.h"
#include "ve_eventhandler.h"
#include "vftestentityspyfilter.h"
#include "vftestcomponentspyfilter.h"
#include "vf-cpp-entity.h"
#include <QAbstractEventDispatcher>
#include <QTest>

QTEST_MAIN(test_modman_start)

static constexpr int systemEntityId = 0;

void test_modman_start::emptyModman()
{
    VeinEvent::EventHandler vfEventHandler;
    VeinTestServer vfTestServer(&vfEventHandler);
    feedEventLoop();

    QList<int> entities = vfTestServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);
    QCOMPARE(VfTestEntitySpyFilter::first(entities, systemEntityId), systemEntityId);

    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfTestServer.getComponentAddList();
    QList<VfTestComponentSpy::TComponentInfo> systemComponents = VfTestComponentSpyFilter::filter(allComponents, systemEntityId);
    QCOMPARE(allComponents.size(), systemComponents.size());

    QCOMPARE(systemComponents.size(), 7);
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "EntityName"));
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "EntityName").newValue, "_System");
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "Session"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "SessionsAvailable"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "Entities"));
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "Entities").newValue, QVariant()); // _SYSTEM listed once ModuleManagerController::initializeEntity is called
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "ModulesPaused"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "Error_Messages"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "LoggedComponents"));

    QCOMPARE(vfTestServer.getComponentChangeList().size(), 0);
}

void test_modman_start::modmanPlusOneEntity()
{
    VeinEvent::EventHandler vfEventHandler;
    VeinTestServer vfTestServer(&vfEventHandler);

    int entityId = 1;
    VfCpp::VfCppEntity entity(entityId);
    vfEventHandler.addSubsystem(&entity);
    entity.initModule();
    entity.createComponent("EntityName", "FooEntity", true);
    entity.createComponent("Foo", "FooVal", false);
    feedEventLoop();

    QList<int> entities = vfTestServer.getEntityAddList();
    QCOMPARE(entities.size(), 2);
    QCOMPARE(VfTestEntitySpyFilter::first(entities, entityId), entityId);

    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfTestServer.getComponentAddList();
    QList<VfTestComponentSpy::TComponentInfo> systemComponents = VfTestComponentSpyFilter::filter(allComponents, systemEntityId);
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "Entities").newValue, QVariant()); // _SYSTEM listed once ModuleManagerController::initializeEntity is called

    QList<VfTestComponentSpy::TComponentInfo> entityComponents = VfTestComponentSpyFilter::filter(allComponents, entityId);
    QCOMPARE(entityComponents.size(), 2);
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "EntityName"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents, "EntityName").newValue, "FooEntity");
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "Foo"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents, "Foo").newValue, "FooVal");
}

void test_modman_start::modmanPlusOneEntityModulesLoaded()
{
    VeinEvent::EventHandler vfEventHandler;
    VeinTestServer vfTestServer(&vfEventHandler);

    int entityId = 1;
    VfCpp::VfCppEntity entity(entityId);
    vfEventHandler.addSubsystem(&entity);
    entity.initModule();
    entity.createComponent("EntityName", "FooEntity", true);
    feedEventLoop();

    vfTestServer.resetLists();
    vfTestServer.simulAllModulesLoaded(QString("fooSession"), QStringList() << "fooSession" << "barSession");
    feedEventLoop();

    QCOMPARE(vfTestServer.getEntityAddList().size(), 0);
    QCOMPARE(vfTestServer.getComponentAddList().size(), 0);
    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfTestServer.getComponentChangeList();
    QList<VfTestComponentSpy::TComponentInfo> systemComponents = VfTestComponentSpyFilter::filter(allComponents, systemEntityId);
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "Session"));
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "Session").newValue, "fooSession");

    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "SessionsAvailable"));
    QVariantList sessionsAvail = VfTestComponentSpyFilter::first(systemComponents, "SessionsAvailable").newValue.toList();
    QVERIFY(sessionsAvail.contains("fooSession"));
    QVERIFY(sessionsAvail.contains("barSession"));

    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "Entities"));

    QVariant entitiesRaw = VfTestComponentSpyFilter::first(systemComponents, "Entities").newValue;
    QList<int> entities = entitiesRaw.value<QList<int>>();
    QVERIFY(!entities.isEmpty());
    QVERIFY(entities.contains(systemEntityId));
    QVERIFY(entities.contains(entityId));
}

void test_modman_start::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
