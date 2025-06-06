#include "test_modman_start.h"
#include "testveinserver.h"
#include "vftestentityspyfilter.h"
#include "vftestcomponentspyfilter.h"
#include <timemachineobject.h>
#include <QTest>

QTEST_MAIN(test_modman_start)

static constexpr int systemEntityId = 0;

void test_modman_start::emptyModman()
{
    TestVeinServer vfTestServer;
    TimeMachineObject::feedEventLoop();

    QList<int> entities = vfTestServer.getEntityAddList();
    QCOMPARE(entities.size(), 1);
    QCOMPARE(VfTestEntitySpyFilter::first(entities, systemEntityId), systemEntityId);

    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfTestServer.getComponentAddList();
    QList<VfTestComponentSpy::TComponentInfo> systemComponents = VfTestComponentSpyFilter::filter(allComponents, systemEntityId);
    QCOMPARE(allComponents.size(), systemComponents.size());

    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "EntityName"));
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "EntityName").newValue, "_System");
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "Session"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "SessionsAvailable"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "XSession"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "Entities"));
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "Entities").newValue, QVariant()); // _SYSTEM listed once SystemModuleEventSystem::initializeEntity is called
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "ModulesPaused"));
    QVERIFY(VfTestComponentSpyFilter::hasOne(systemComponents, "DevMode"));

    QCOMPARE(vfTestServer.getComponentChangeList().size(), 0);
}

void test_modman_start::modmanPlusOneEntity()
{
    TestVeinServer vfTestServer;

    int entityId = 1;
    vfTestServer.addEntity(entityId, "FooEntity");
    vfTestServer.addComponent(entityId, "Foo", "FooVal", false);
    TimeMachineObject::feedEventLoop();

    QList<int> entities = vfTestServer.getEntityAddList();
    QCOMPARE(entities.size(), 2);
    QCOMPARE(VfTestEntitySpyFilter::first(entities, entityId), entityId);

    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfTestServer.getComponentAddList();
    QList<VfTestComponentSpy::TComponentInfo> systemComponents = VfTestComponentSpyFilter::filter(allComponents, systemEntityId);
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "Entities").newValue, QVariant()); // _SYSTEM listed once SystemModuleEventSystem::initializeEntity is called

    QList<VfTestComponentSpy::TComponentInfo> entityComponents = VfTestComponentSpyFilter::filter(allComponents, entityId);
    QCOMPARE(entityComponents.size(), 2);
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "EntityName"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents, "EntityName").newValue, "FooEntity");
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents, "Foo"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents, "Foo").newValue, "FooVal");
}

void test_modman_start::modmanPlusTwoEntities()
{
    TestVeinServer vfTestServer;

    int entityId1 = 1;
    vfTestServer.addEntity(entityId1, "FooEntity");
    vfTestServer.addComponent(entityId1, "Foo", "FooVal", false);
    int entityId2 = 2;
    vfTestServer.addEntity(entityId2, "BarEntity");
    vfTestServer.addComponent(entityId2, "Bar", "BarVal", false);
    vfTestServer.addComponent(entityId2, "Baz", "BazVal", false);
    TimeMachineObject::feedEventLoop();

    QList<int> entities = vfTestServer.getEntityAddList();
    QCOMPARE(entities.size(), 3);
    QCOMPARE(VfTestEntitySpyFilter::first(entities, entityId1), entityId1);
    QCOMPARE(VfTestEntitySpyFilter::first(entities, entityId2), entityId2);

    QList<VfTestComponentSpy::TComponentInfo> allComponents = vfTestServer.getComponentAddList();
    QList<VfTestComponentSpy::TComponentInfo> systemComponents = VfTestComponentSpyFilter::filter(allComponents, systemEntityId);
    QCOMPARE(VfTestComponentSpyFilter::first(systemComponents, "Entities").newValue, QVariant()); // _SYSTEM listed once SystemModuleEventSystem::initializeEntity is called

    QList<VfTestComponentSpy::TComponentInfo> entityComponents1 = VfTestComponentSpyFilter::filter(allComponents, entityId1);
    QCOMPARE(entityComponents1.size(), 2);
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents1, "EntityName"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents1, "EntityName").newValue, "FooEntity");
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents1, "Foo"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents1, "Foo").newValue, "FooVal");

    QList<VfTestComponentSpy::TComponentInfo> entityComponents2 = VfTestComponentSpyFilter::filter(allComponents, entityId2);
    QCOMPARE(entityComponents2.size(), 3);
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents2, "EntityName"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents2, "EntityName").newValue, "BarEntity");
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents2, "Bar"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents2, "Bar").newValue, "BarVal");
    QVERIFY(VfTestComponentSpyFilter::hasOne(entityComponents2, "Baz"));
    QCOMPARE(VfTestComponentSpyFilter::first(entityComponents2, "Baz").newValue, "BazVal");
}

void test_modman_start::modmanPlusOneEntityModulesLoaded()
{
    TestVeinServer vfTestServer;

    int entityId = 1;
    vfTestServer.addEntity(entityId, "FooEntity");
    vfTestServer.addComponent(entityId, "Foo", "FooVal", false);
    TimeMachineObject::feedEventLoop();

    vfTestServer.resetEventSpyData();
    vfTestServer.simulAllModulesLoaded(QString("fooSession"), QStringList() << "fooSession" << "barSession");

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
