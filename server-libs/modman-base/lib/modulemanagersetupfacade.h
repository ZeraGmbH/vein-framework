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
    ModuleManagerSetupFacade(LicenseSystemInterface *licenseSystem,
                             bool devMode = false,
                             const LxdmSessionChangeParam& lxdmParam = LxdmSessionChangeParam(),
                             const QString persistencyBasePath = "");
    static void registerMetaTypeStreamOperators();

    void prependModuleSystem(VeinEvent::EventSystem* subsystem);   // prepend to module event systems (topmost)
    void addModuleSystem(VeinEvent::EventSystem* system);          // append to module event system
    void addSubsystem(VeinEvent::EventSystem* subsystem);          // append to common event systems (bottommost)
    void clearModuleSystems();
    SystemModuleEventSystem *getSystemModuleEventSystem();
    VeinStorage::AbstractEventSystem *getStorageSystem();
    ModuleEventHandler *getEventHandler();

    LicenseSystemInterface *getLicenseSystem();
    QString getPersistencyBasePath();
private:
    ModuleEventHandler m_eventHandler;
    VeinStorage::StorageEventSystem m_storSystem;
    SystemModuleEventSystem m_systemModuleSystem;
    VeinNet::IntrospectionSystem m_introspectionSystem;
    LicenseSystemInterface *m_licenseSystem = nullptr;
    QString m_persistencyBasePath;
};

#endif // MODULEMANAGERSETUPFACADE_H
