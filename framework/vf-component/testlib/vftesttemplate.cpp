#include "vftesttemplate.h"
#include <QCoreApplication>

void VfTestTemplate::addEntityComponent(int entityId, QString componentName)
{
    m_vfEntity->createEntityComponent(entityId, componentName);
    m_vfListener->addComponentFilter(entityId, componentName);
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

    QCoreApplication::processEvents();
}

void VfTestTemplate::_cleanup()
{
    delete m_vfEventHandler;
    delete m_vfListener;
    delete m_vfComponentData;
    delete m_vfEntity;
}
