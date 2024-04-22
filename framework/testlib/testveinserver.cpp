#include "testveinserver.h"
#include "vf_client_component_setter.h"
#include "modulemanagersetupfacade.h"
#include "testdumpreporter.h"
#include <timemachineobject.h>
#include <QBuffer>

using VeinComponent::EntityData;
using VeinComponent::ComponentData;

TestVeinServer::TestVeinServer() :
    m_vfEntityAddSpy(EntityData::Command::ECMD_ADD),
    m_vfComponentAddSpy(ComponentData::Command::CCMD_ADD),
    m_vfComponentChangeSpy(ComponentData::Command::CCMD_SET),
    m_serverCmdEventSpyTop(&m_JsonEventLog, "server-enter"),
    m_serverCmdEventSpyBottom(&m_JsonEventLog, "server-fallthrough")

{
    ModuleManagerSetupFacade::registerMetaTypeStreamOperators();

    m_systemModuleSystem.setStorage(&m_storageSystem);

    m_vfEventHandler.addSubsystem(&m_serverCmdEventSpyTop);
    m_vfEventHandler.addSubsystem(&m_vfEntityAddSpy);
    m_vfEventHandler.addSubsystem(&m_vfComponentAddSpy);
    m_vfEventHandler.addSubsystem(&m_vfComponentChangeSpy);
    m_vfEventHandler.addSubsystem(&m_systemModuleSystem);
    m_vfEventHandler.addSubsystem(&m_introspectionSystem);
    m_vfEventHandler.addSubsystem(&m_storageSystem);
    m_vfEventHandler.addSubsystem(&m_serverCmdEventSpyBottom);
    QObject::connect(&m_vfEventHandler, &VeinEvent::EventHandler::sigEventAccepted,
                     &m_serverCmdEventSpyBottom, &TestJsonSpyEventSystem::onEventAccepted);

    m_systemModuleSystem.initOnce();
}

void TestVeinServer::prependEventSystem(VeinEvent::EventSystem *system)
{
    m_vfEventHandler.prependSubsystem(system);
    // Ensure topmost
    m_vfEventHandler.removeSubsystem(&m_serverCmdEventSpyTop);
    m_vfEventHandler.prependSubsystem(&m_serverCmdEventSpyTop);
}

void TestVeinServer::appendEventSystem(VeinEvent::EventSystem *system)
{
    m_vfEventHandler.addSubsystem(system);
    // Ensure bottommost
    m_vfEventHandler.removeSubsystem(&m_serverCmdEventSpyBottom);
    m_vfEventHandler.addSubsystem(&m_serverCmdEventSpyBottom);
}

void TestVeinServer::addTestEntities(int entityCount, int componentCount, const int baseEntityId, int baseComponentNum)
{
    for(int entityId = baseEntityId; entityId<baseEntityId+entityCount; entityId++) {
        QString entityName = QString("EntityName%1").arg(entityId);
        addEntity(entityId, entityName);
        for(int component=baseComponentNum; component<baseComponentNum+componentCount; component++) {
            QString componentName = QString("ComponentName%1").arg(component);
            addComponent(entityId, componentName, QVariant(), false);
        }
    }
}

void TestVeinServer::addEntity(int entityId, QString entityName)
{
    if(m_entities.find(entityId) != m_entities.end())
        qFatal("Entity ID %i already inserted!", entityId);

    m_entities[entityId] = std::make_unique<VfCpp::VfCppEntity>(entityId);
    appendEventSystem(m_entities[entityId].get());
    m_entities[entityId]->initModule();
    m_entities[entityId]->createComponent("EntityName", entityName, true);
    TimeMachineObject::feedEventLoop();
}

void TestVeinServer::addComponent(int entityId, QString componentName, QVariant initialValue, bool readOnly)
{
    if(m_entities.find(entityId) == m_entities.end())
        qFatal("Entity with ID %i was not added by addEntity!", entityId);
    m_entities[entityId]->createComponent(componentName, initialValue, readOnly);
    TimeMachineObject::feedEventLoop();
}

void TestVeinServer::setComponent(int entityId, QString componentName, QVariant newValue)
{
    if(!m_storageSystem.hasStoredValue(entityId, componentName))
        qFatal("No entity/component with ID %i / Name %s available!", entityId, qPrintable(componentName));

    QVariant oldValue = m_storageSystem.getStoredValue(entityId, componentName);
    QEvent* event = VfClientComponentSetter::generateEvent(entityId, componentName, oldValue, newValue);
    sendEvent(event);
    TimeMachineObject::feedEventLoop();
}

void TestVeinServer::removeEntitiesAdded()
{
    for(const auto &entity : m_entities) {
        entity.second->prepareRemove();
        m_vfEventHandler.removeSubsystem(entity.second.get());
    }
    m_entities.clear();
    TimeMachineObject::feedEventLoop();
}

QMap<int, QList<QString>> TestVeinServer::getTestEntityComponentInfo()
{
    QMap<int, QList<QString>> info;
    for(const auto &entity : m_entities) {
        int entityId = entity.first;
        QMap<QString, VfCpp::VfCppComponent::Ptr> componentList = entity.second->getComponents();
        QStringList componentNames = componentList.keys();
        info[entityId] = componentNames;
    }
    return info;
}

VeinEvent::EventHandler *TestVeinServer::getEventHandler()
{
    return &m_vfEventHandler;
}

void TestVeinServer::sendEvent(QEvent *event)
{
    emit m_storageSystem.sigSendEvent(event);
}

VeinEvent::StorageSystem *TestVeinServer::getStorage()
{
    return &m_storageSystem;
}

QByteArray TestVeinServer::dumpStorage(QList<int> entities)
{
    QByteArray jsonDumped;
    QBuffer buff(&jsonDumped);
    m_storageSystem.dumpToFile(&buff, entities);
    return jsonDumped;
}

QByteArray TestVeinServer::dumpEvents()
{
    return TestDumpReporter::dump(m_JsonEventLog);
}

QList<int> TestVeinServer::getEntityAddList() const
{
    return m_vfEntityAddSpy.getEntityList();
}

QList<VfTestComponentSpy::TComponentInfo> TestVeinServer::getComponentAddList() const
{
    return m_vfComponentAddSpy.getComponentList();
}

QList<VfTestComponentSpy::TComponentInfo> TestVeinServer::getComponentChangeList() const
{
    return m_vfComponentChangeSpy.getComponentList();
}

void TestVeinServer::simulAllModulesLoaded(const QString &sessionPath, const QStringList &sessionList)
{
    m_systemModuleSystem.initializeEntity(sessionPath, sessionList);
    TimeMachineObject::feedEventLoop();
}

void TestVeinServer::resetEventSpyData()
{
    m_vfEntityAddSpy.reset();
    m_vfComponentAddSpy.reset();
    m_vfComponentChangeSpy.reset();
    m_JsonEventLog = QJsonObject();
}
