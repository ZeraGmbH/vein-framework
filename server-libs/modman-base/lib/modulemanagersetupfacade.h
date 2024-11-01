#ifndef MODULEMANAGERSETUPFACADE_H
#define MODULEMANAGERSETUPFACADE_H

#include "licensesysteminterface.h"
#include "moduleeventhandler.h"
#include "systemmoduleeventsystem.h"
#include "vn_introspectionsystem.h"
#include "vs_storageeventsystem.h"

class ModuleManagerSetupFacade : public QObject
{
    Q_OBJECT
public:
    ModuleManagerSetupFacade(LicenseSystemInterface *licenseSystem, bool devMode = false, QObject *parent = nullptr);
    static void registerMetaTypeStreamOperators();
    void addSubsystem(VeinEvent::EventSystem* subsystem);
    void addModuleSystem(VeinEvent::EventSystem* system);
    void clearModuleSystems();
    SystemModuleEventSystem *getSystemModuleEventSystem();
    VeinStorage::AbstractEventSystem *getStorageSystem();
    LicenseSystemInterface *getLicenseSystem();
private:
    ModuleEventHandler m_eventHandler;
    VeinStorage::StorageEventSystem m_storSystem;
    SystemModuleEventSystem m_systemModuleSystem;
    VeinNet::IntrospectionSystem m_introspectionSystem;
    LicenseSystemInterface *m_licenseSystem = nullptr;
};

#endif // MODULEMANAGERSETUPFACADE_H
