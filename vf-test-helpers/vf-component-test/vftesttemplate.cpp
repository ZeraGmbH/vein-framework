#include "vftesttemplate.h"
#include <QCoreApplication>

void VfTestTemplate::addSecondEntity(QString compName)
{
    m_vfEntity->createEntityComponent(entityId+1, compName);
    m_vfListener->addComponentToListen(entityId+1, compName);
    QCoreApplication::processEvents();
}

void VfTestTemplate::_init()
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

void VfTestTemplate::_cleanup()
{
    delete m_vfEventHandler;
    delete m_vfListener;
    delete m_vfComponentData;
    delete m_vfEntity;
}
