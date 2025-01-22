#include "modulemanagersetupfacade.h"
#include <QDataStream>

ModuleManagerSetupFacade::ModuleManagerSetupFacade(LicenseSystemInterface *licenseSystem,
                                                   bool devMode,
                                                   const LxdmSessionChangeParam &lxdmParam) :
    m_systemModuleSystem(devMode, lxdmParam),
    m_licenseSystem(licenseSystem)
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

void ModuleManagerSetupFacade::addSubsystem(VeinEvent::EventSystem *subsystem)
{
    m_eventHandler.addSubsystem(subsystem);
}

void ModuleManagerSetupFacade::addModuleSystem(VeinEvent::EventSystem *system)
{
    m_eventHandler.addModuleSystem(system);
}

void ModuleManagerSetupFacade::clearModuleSystems()
{
    m_eventHandler.clearModuleSystems();
}

SystemModuleEventSystem *ModuleManagerSetupFacade::getSystemModuleEventSystem()
{
    return &m_systemModuleSystem;
}

VeinStorage::AbstractEventSystem *ModuleManagerSetupFacade::getStorageSystem()
{
    return &m_storSystem;
}

LicenseSystemInterface *ModuleManagerSetupFacade::getLicenseSystem()
{
    return m_licenseSystem;
}
