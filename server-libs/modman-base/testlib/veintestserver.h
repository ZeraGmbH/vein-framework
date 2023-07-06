#ifndef VEINTESTSERVER_H
#define VEINTESTSERVER_H

#include "modulemanagercontroller.h"
#include "ve_eventhandler.h"
#include "vn_introspectionsystem.h"
#include "vs_veinhash.h"
#include "vftestentityspy.h"
#include "vftestcomponentspy.h"

class VeinTestServer
{
public:
    explicit VeinTestServer(VeinEvent::EventHandler *eventHandler);
    QList<int> getEntityAddList() const;
    QList<VfTestComponentSpy::TComponentInfo> getComponentAddList() const;
    QList<VfTestComponentSpy::TComponentInfo> getComponentChangeList() const;
private:
    VeinEvent::EventHandler* m_vfEventHandler;
    VfTestEntitySpy m_vfEntityAddSpy;
    VfTestComponentSpy m_vfComponentAddSpy;
    VfTestComponentSpy m_vfComponentChangeSpy;
    ModuleManagerController m_mmController;
    VeinNet::IntrospectionSystem m_introspectionSystem;
    VeinStorage::VeinHash m_storageSystem;
};

#endif // VEINTESTSERVER_H
