#include "test_vfcomponentshoutandlisten.h"
#include <QTest>

static const char* entityName = "fooName";

void test_VfComponentShoutAndListen::init()
{
    m_vfStorageHash = new VeinStorage::VeinHash;
    m_vfEntity = new VfTestEntity;
    m_vfShouter = new VfTestComponentShouter;
    m_vfListener = new VfTestComponentListener(m_vfStorageHash);
    m_vfEventHandler = new VeinEvent::EventHandler;

    m_vfEventHandler->addSubsystem(m_vfShouter);
    m_vfEventHandler->addSubsystem(m_vfEntity);
    m_vfEventHandler->addSubsystem(m_vfListener);
    m_vfEventHandler->addSubsystem(m_vfStorageHash);

    m_vfListener->addComponentToNotify(entityName, &m_vfShouter->getValue());

    m_vfEntity->createEntity(entityId);
    QCoreApplication::processEvents();
}

void test_VfComponentShoutAndListen::cleanup()
{
    delete m_vfEventHandler;
    delete m_vfListener;
    delete m_vfShouter;
    delete m_vfEntity;
    delete m_vfStorageHash;
}

void test_VfComponentShoutAndListen::listenerEmpty()
{
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 0);
}

void test_VfComponentShoutAndListen::shoutAndReceiveOne()
{
    m_vfShouter->setValue(entityId, entityName, QVariant(1));
    QCoreApplication::processEvents();
    QCOMPARE(m_vfListener->getComponentChangeList().count(), 1);
}

QTEST_MAIN(test_VfComponentShoutAndListen)
