#include "test_vfcomponent.h"
#include <QTest>

static constexpr int entityId = 1;
const char* componentName = "fooName";

void test_vfcomponent::init()
{
    _init();
    addEntityComponent(entityId, componentName);
}

void test_vfcomponent::cleanup()
{
    _cleanup();
}

void test_vfcomponent::listenerEmpty()
{
    QCOMPARE(m_vfListener->getComponentList().count(), 0);
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
    QCOMPARE(m_vfListener->getComponentList().count(), 1);
}

void test_vfcomponent::twoChangesOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(2));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentList().count(), 2);
}

void test_vfcomponent::twoIdenticalChangesOneEntity()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentList().count(), 1);
}

void test_vfcomponent::oneChangeTwoEntities()
{
    addEntityComponent(entityId+1, componentName);
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentList().count(), 1);
}

void test_vfcomponent::twoChangeTwoEntities()
{
    addEntityComponent(entityId+1, componentName);
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentList().count(), 2);
}

void test_vfcomponent::twoChangeTwoEntitiesDiffCompName()
{
    addEntityComponent(entityId+1, "bar");
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, "bar", QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentList().count(), 2);
}

void test_vfcomponent::twoChangeTwoEntitiesCheckContents()
{
    addEntityComponent(entityId+1, "bar");
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId+1, "bar", QVariant(2));
    QCoreApplication::processEvents();
    QList<VfTestComponentSpy::TComponentInfo> list = m_vfListener->getComponentList();
    QCOMPARE(list.count(), 2);
    QCOMPARE(list[0].componentName, componentName);
    QCOMPARE(list[0].entityId, entityId);
    QCOMPARE(list[0].newValue, QVariant(1));
    QCOMPARE(list[1].componentName, "bar");
    QCOMPARE(list[1].entityId, entityId+1);
    QCOMPARE(list[1].newValue, QVariant(2));
}

QTEST_MAIN(test_vfcomponent)
