#include "veintestserver.h"

using VeinComponent::EntityData;
using VeinComponent::ComponentData;

VeinTestServer::VeinTestServer(VeinEvent::EventHandler *eventHandler) :
    m_vfEventHandler(eventHandler),
    m_vfEntityAddSpy(EntityData::Command::ECMD_ADD),
    m_vfComponentAddSpy(ComponentData::Command::CCMD_ADD),
    m_vfComponentChangeSpy(ComponentData::Command::CCMD_SET)

{
    m_systemModuleSystem.setStorage(&m_storageSystem);

    m_vfEventHandler->addSubsystem(&m_vfEntityAddSpy);
    m_vfEventHandler->addSubsystem(&m_vfComponentAddSpy);
    m_vfEventHandler->addSubsystem(&m_vfComponentChangeSpy);
    m_vfEventHandler->addSubsystem(&m_systemModuleSystem);
    m_vfEventHandler->addSubsystem(&m_introspectionSystem);
    m_vfEventHandler->addSubsystem(&m_storageSystem);

    m_systemModuleSystem.initOnce();
}

void VeinTestServer::addEntity(int entityId, QString entityName)
{
    if(m_entities.find(entityId) != m_entities.end()) {
        qFatal("Entity ID %i already inserted!", entityId);
    }
    m_entities[entityId] = std::make_unique<VfCpp::VfCppEntity>(entityId);
    m_vfEventHandler->addSubsystem(m_entities[entityId].get());
    m_entities[entityId]->initModule();
    m_entities[entityId]->createComponent("EntityName", entityName, true);
}

void VeinTestServer::addComponent(int entityId, QString componentName, QVariant initialValue, bool readOnly)
{
    if(m_entities.find(entityId) == m_entities.end())
        qFatal("Entity with ID %i was not added by addEntity!", entityId);
    m_entities[entityId]->createComponent(componentName, initialValue, readOnly);
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

void VeinTestServer::simulAllModulesLoaded(const QString &sessionPath, const QStringList &sessionList)
{
    m_systemModuleSystem.initializeEntity(sessionPath, sessionList);
}

void VeinTestServer::resetLists()
{
    m_vfEntityAddSpy.reset();
    m_vfComponentAddSpy.reset();
    m_vfComponentChangeSpy.reset();
}
