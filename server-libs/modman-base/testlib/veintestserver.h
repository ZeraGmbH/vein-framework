#ifndef VEINTESTSERVER_H
#define VEINTESTSERVER_H

#include "systemmoduleeventsystem.h"
#include "ve_eventhandler.h"
#include "vf-cpp-entity.h"
#include "vn_introspectionsystem.h"
#include "vs_veinhash.h"
#include "vftestentityspy.h"
#include "vftestcomponentspy.h"
#include <memory>
#include <unordered_map>

class VeinTestServer
{
public:
    explicit VeinTestServer(VeinEvent::EventHandler *eventHandler);
    void addEntity(int entityId, QString entityName);
    void addComponent(int entityId, QString componentName, QVariant initialValue, bool readOnly);
    QList<int> getEntityAddList() const;
    QList<VfTestComponentSpy::TComponentInfo> getComponentAddList() const;
    QList<VfTestComponentSpy::TComponentInfo> getComponentChangeList() const;
    void simulAllModulesLoaded(const QString &sessionPath, const QStringList &sessionList);
    void resetLists();
private:
    VeinEvent::EventHandler* m_vfEventHandler;
    VfTestEntitySpy m_vfEntityAddSpy;
    VfTestComponentSpy m_vfComponentAddSpy;
    VfTestComponentSpy m_vfComponentChangeSpy;
    SystemModuleEventSystem m_systemModuleSystem;
    VeinNet::IntrospectionSystem m_introspectionSystem;
    VeinStorage::VeinHash m_storageSystem;

    std::unordered_map<int, std::unique_ptr<VfCpp::VfCppEntity>> m_entities;
};

#endif // VEINTESTSERVER_H
