#include "test_vftestcomponentbareclasses.h"
#include <QTest>

static const char* componentName = "fooName";

void test_vftestcomponentbareclasses::init()
{
    m_vfEntity = new VfTestEntityComponentCreator;
    m_vfComponentData = new VfTestComponentData;
    m_vfListener = new VfTestComponentChangeListener;
    m_vfEventHandler = new VeinEvent::EventHandler;

    m_vfEventHandler->addSubsystem(m_vfComponentData);
    m_vfEventHandler->addSubsystem(m_vfEntity);
    m_vfEventHandler->addSubsystem(m_vfListener);

    m_vfEntity->createEntityComponent(entityId, componentName);
    m_vfListener->addComponentToListen(entityId, componentName);

    QCoreApplication::processEvents();
}

void test_vftestcomponentbareclasses::cleanup()
{
    delete m_vfEventHandler;
    delete m_vfListener;
    delete m_vfComponentData;
    delete m_vfEntity;
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

void test_vftestcomponentbareclasses::oneChange()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vftestcomponentbareclasses::twoChange()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(2));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 2);
}

void test_vftestcomponentbareclasses::twoChangeSame()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

void test_vftestcomponentbareclasses::ignoreUnlistened()
{
    m_vfComponentData->setValue(entityId, "", QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

QTEST_MAIN(test_vftestcomponentbareclasses)
