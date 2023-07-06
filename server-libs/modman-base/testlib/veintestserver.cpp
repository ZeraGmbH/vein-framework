#include "veintestserver.h"

using VeinComponent::EntityData;
using VeinComponent::ComponentData;

VeinTestServer::VeinTestServer(VeinEvent::EventHandler *eventHandler) :
    m_vfEventHandler(eventHandler),
    m_vfEntityAddSpy(EntityData::Command::ECMD_ADD),
    m_vfComponentAddSpy(ComponentData::Command::CCMD_ADD),
    m_vfComponentChangeSpy(ComponentData::Command::CCMD_SET)

{
    m_mmController.setStorage(&m_storageSystem);

    m_vfEventHandler->addSubsystem(&m_vfEntityAddSpy);
    m_vfEventHandler->addSubsystem(&m_vfComponentAddSpy);
    m_vfEventHandler->addSubsystem(&m_vfComponentChangeSpy);
    m_vfEventHandler->addSubsystem(&m_mmController);
    m_vfEventHandler->addSubsystem(&m_introspectionSystem);
    m_vfEventHandler->addSubsystem(&m_storageSystem);

    m_mmController.initOnce();
}

QList<int> VeinTestServer::getEntityAddList() const
{
    return m_vfEntityAddSpy.getEntityList();
}

QList<VfTestComponentSpy::TComponentInfo> VeinTestServer::getComponentAddList() const
{
    return m_vfComponentAddSpy.getComponentList();
}

QList<VfTestComponentSpy::TComponentInfo> VeinTestServer::getComponentChangeList() const
{
    return m_vfComponentChangeSpy.getComponentList();
}
