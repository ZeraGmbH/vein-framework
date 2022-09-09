#include "test_vfcomponent.h"
#include <QTest>

void test_vfcomponent::init()
{
    _init();
}

void test_vfcomponent::cleanup()
{
    _cleanup();
}

void test_vfcomponent::listenerEmpty()
{
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_vfcomponent::detectDoubleEntityComponents()
{
    QVERIFY(m_vfEntity->createEntityComponent(1, "foo"));
    QVERIFY(!m_vfEntity->createEntityComponent(1, "foo"));
}

void test_vfcomponent::oneChangeOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vfcomponent::twoChangesOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(2));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 2);
}

void test_vfcomponent::twoIdenticalChangesOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vfcomponent::ignoreUnlistenedComponent()
{
    m_vfComponentData->setValue(entityId, "", QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_vfcomponent::ignoreUnlistenedEntity()
{
    m_vfComponentData->setValue(2, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_vfcomponent::oneChangeTwoEntities()
{
    addSecondEntity(componentName);
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vfcomponent::twoChangeTwoEntities()
{
    addSecondEntity(componentName);
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 2);
}

void test_vfcomponent::twoChangeTwoEntitiesDiffCompName()
{
    addSecondEntity("bar");
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, "bar", QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 2);
}

void test_vfcomponent::twoChangeTwoEntitiesCheckContents()
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

QTEST_MAIN(test_vfcomponent)
