#include "modulemanagersetupfacade.h"
#include <QDataStream>

ModuleManagerSetupFacade::ModuleManagerSetupFacade(LicenseSystemInterface *licenseSystem,
                                                   bool devMode,
                                                   const LxdmSessionChangeParam &lxdmParam,
                                                   const QString persistencyBasePath) :
    m_systemModuleSystem(devMode, lxdmParam),
    m_introspectionSystem(&m_storSystem),
    m_licenseSystem(licenseSystem),
    m_persistencyBasePath(persistencyBasePath)
{
    Q_ASSERT(m_licenseSystem != nullptr);
    m_eventHandler.addSubsystem(&m_systemModuleSystem);
    m_eventHandler.addSubsystem(&m_introspectionSystem);
    m_eventHandler.addSubsystem(&m_storSystem);
    m_eventHandler.addSubsystem(m_licenseSystem);
    m_systemModuleSystem.setStorage(&m_storSystem);
}

void ModuleManagerSetupFacade::registerMetaTypeStreamOperators()
{
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
    qRegisterMetaTypeStreamOperators<QList<float> >("QList<float>");
    qRegisterMetaTypeStreamOperators<QList<double> >("QList<double>");
    qRegisterMetaTypeStreamOperators<QList<QString> >("QList<QString>");
    qRegisterMetaTypeStreamOperators<QVector<QString> >("QVector<QString>");
    qRegisterMetaTypeStreamOperators<QList<QVariantMap> >("QList<QVariantMap>");
}

void ModuleManagerSetupFacade::prependModuleSystem(VeinEvent::EventSystem *subsystem)
{
    m_eventHandler.prependModuleSystem(subsystem);
}

void ModuleManagerSetupFacade::addModuleSystem(VeinEvent::EventSystem *system)
{
    m_eventHandler.addModuleSystem(system);
}

void ModuleManagerSetupFacade::addSubsystem(VeinEvent::EventSystem *subsystem)
{
    m_eventHandler.addSubsystem(subsystem);
}

void ModuleManagerSetupFacade::clearModuleSystems()
{
    m_eventHandler.clearModuleSystems();
}

void ModuleManagerSetupFacade::clearSubSystem(VeinEvent::EventSystem *subsystem)
{
    m_eventHandler.removeSubsystem(subsystem);
}

SystemModuleEventSystem *ModuleManagerSetupFacade::getSystemModuleEventSystem()
{
    return &m_systemModuleSystem;
}

VeinStorage::AbstractEventSystem *ModuleManagerSetupFacade::getStorageSystem()
{
    return &m_storSystem;
}

ModuleEventHandler *ModuleManagerSetupFacade::getEventHandler()
{
    return &m_eventHandler;
}

LicenseSystemInterface *ModuleManagerSetupFacade::getLicenseSystem()
{
    return m_licenseSystem;
}

QString ModuleManagerSetupFacade::getPersistencyBasePath()
{
    return m_persistencyBasePath;
}
