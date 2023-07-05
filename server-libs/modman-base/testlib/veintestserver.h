#ifndef VEINTESTSERVER_H
#define VEINTESTSERVER_H

#include "modulemanagercontroller.h"
#include "ve_eventhandler.h"
#include "vn_introspectionsystem.h"
#include "vs_veinhash.h"
#include "vftestcomponentspy.h"

class VeinTestServer
{
public:
    explicit VeinTestServer(VeinEvent::EventHandler *eventHandler);
    QList<VfTestComponentSpy::TComponentInfo> getComponentAddList();
    QList<VfTestComponentSpy::TComponentInfo> getComponentChangeList();
private:
    VeinEvent::EventHandler* m_vfEventHandler;
    VfTestComponentSpy m_vfAddListener;
    VfTestComponentSpy m_vfChangeListener;
    ModuleManagerController m_mmController;
    VeinNet::IntrospectionSystem m_introspectionSystem;
    VeinStorage::VeinHash m_storageSystem;
};

#endif // VEINTESTSERVER_H
