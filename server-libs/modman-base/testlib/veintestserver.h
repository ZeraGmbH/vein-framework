#ifndef VEINTESTSERVER_H
#define VEINTESTSERVER_H

#include "modulemanagercontroller.h"
#include "ve_eventhandler.h"
#include "vn_introspectionsystem.h"
#include "vs_veinhash.h"


class VeinTestServer : public QObject
{
    Q_OBJECT
public:
    explicit VeinTestServer(VeinEvent::EventHandler *eventHandler);

signals:

private:
    VeinEvent::EventHandler* m_vfEventHandler;
    ModuleManagerController m_mmController;
    VeinNet::IntrospectionSystem m_introspectionSystem;
    VeinStorage::VeinHash m_storageSystem;
};

#endif // VEINTESTSERVER_H
