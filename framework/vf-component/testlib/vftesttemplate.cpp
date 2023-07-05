#include "vftesttemplate.h"
#include <QCoreApplication>

void VfTestTemplate::addEntityComponent(int entityId, QString componentName)
{
    m_vfEntity->createEntityComponent(entityId, componentName);
    QCoreApplication::processEvents();
}

using VeinComponent::ComponentData;

void VfTestTemplate::_init()
{
    m_vfEntity = new VfTestEntityComponentCreator;
    m_vfComponentData = new VfTestComponentData;
    m_vfListener = new VfTestComponentSpy(ComponentData::Command::CCMD_SET);
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
