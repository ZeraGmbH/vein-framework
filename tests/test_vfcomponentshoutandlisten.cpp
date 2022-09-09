#include "test_vfcomponentshoutandlisten.h"
#include <QTest>

static const char* componentName = "fooName";

void test_VfComponentShoutAndListen::init()
{
    m_vfStorageHash = new VeinStorage::VeinHash;
    m_vfEntity = new VfTestEntityComponentCreator;
    m_vfComponentData = new VfTestComponentData;
    m_vfListener = new VfTestComponentChangeListener(m_vfStorageHash);
    m_vfEventHandler = new VeinEvent::EventHandler;

    m_vfEventHandler->addSubsystem(m_vfComponentData);
    m_vfEventHandler->addSubsystem(m_vfEntity);
    m_vfEventHandler->addSubsystem(m_vfListener);
    m_vfEventHandler->addSubsystem(m_vfStorageHash);

    m_vfEntity->createEntityComponent(entityId, componentName);
    m_vfListener->addComponentToListen(componentName, &m_vfComponentData->getValue());

    QCoreApplication::processEvents();
}

void test_VfComponentShoutAndListen::cleanup()
{
    delete m_vfEventHandler;
    delete m_vfListener;
    delete m_vfComponentData;
    delete m_vfEntity;
    delete m_vfStorageHash;
}

void test_VfComponentShoutAndListen::listenerEmpty()
{
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_VfComponentShoutAndListen::shoutAndReceiveOne()
{
    m_vfComponentData->setValue(entityId, componentName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

QTEST_MAIN(test_VfComponentShoutAndListen)
