#include "test_vftestcomponentbareclasses.h"
#include <QTest>

void test_vftestcomponentbareclasses::init()
{
    _init();
}

void test_vftestcomponentbareclasses::cleanup()
{
    _cleanup();
}

void test_vftestcomponentbareclasses::listenerEmpty()
{
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_vftestcomponentbareclasses::detectDoubleEntityComponents()
{
    QVERIFY(m_vfEntity->createEntityComponent(1, "foo"));
    QVERIFY(!m_vfEntity->createEntityComponent(1, "foo"));
}

void test_vftestcomponentbareclasses::oneChangeOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vftestcomponentbareclasses::twoChangesOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(2));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 2);
}

void test_vftestcomponentbareclasses::twoIdenticalChangesOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vftestcomponentbareclasses::ignoreUnlistenedComponent()
{
    m_vfComponentData->setValue(entityId, "", QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_vftestcomponentbareclasses::ignoreUnlistenedEntity()
{
    m_vfComponentData->setValue(2, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_vftestcomponentbareclasses::oneChangeTwoEntities()
{
    addSecondEntity(componentName);
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vftestcomponentbareclasses::twoChangeTwoEntities()
{
    addSecondEntity(componentName);
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 2);
}

void test_vftestcomponentbareclasses::twoChangeTwoEntitiesDiffCompName()
{
    addSecondEntity("bar");
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, "bar", QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 2);
}

void test_vftestcomponentbareclasses::twoChangeTwoEntitiesCheckContents()
{
    addSecondEntity("bar");
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, "bar", QVariant(2));
    QCoreApplication::processEvents();
    QList<VfTestComponentChangeListener::TComponentInfo> list = m_vfListener->getComponentChangeList();
    QCOMPARE(list.count(), 2);
    QCOMPARE(list[0].componentName, componentName);
    QCOMPARE(list[0].entityId, entityId);
    QCOMPARE(list[0].value, QVariant(1));
    QCOMPARE(list[1].componentName, "bar");
    QCOMPARE(list[1].entityId, entityId+1);
    QCOMPARE(list[1].value, QVariant(2));
}

QTEST_MAIN(test_vftestcomponentbareclasses)
