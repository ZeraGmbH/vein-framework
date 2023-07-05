#include "veintestserver.h"

using VeinComponent::ComponentData;

VeinTestServer::VeinTestServer(VeinEvent::EventHandler *eventHandler) :
    m_vfEventHandler(eventHandler),
    m_vfAddListener(ComponentData::Command::CCMD_ADD),
    m_vfChangeListener(ComponentData::Command::CCMD_SET)

{
    m_mmController.setStorage(&m_storageSystem);

    m_vfEventHandler->addSubsystem(&m_vfAddListener);
    m_vfEventHandler->addSubsystem(&m_vfChangeListener);
    m_vfEventHandler->addSubsystem(&m_mmController);
    m_vfEventHandler->addSubsystem(&m_introspectionSystem);
    m_vfEventHandler->addSubsystem(&m_storageSystem);

    m_mmController.initOnce();
}

QList<VfTestComponentSpy::TComponentInfo> VeinTestServer::getComponentAddList()
{
    return m_vfAddListener.getComponentList();
}

QList<VfTestComponentSpy::TComponentInfo> VeinTestServer::getComponentChangeList()
{
    return m_vfChangeListener.getComponentList();
}
