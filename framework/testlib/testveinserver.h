#ifndef TESTVEINSERVER_H
#define TESTVEINSERVER_H

#include "systemmoduleeventsystem.h"
#include "testjsonspyeventsystem.h"
#include "lxdmsessionchangeparam.h"
#include "ve_eventhandler.h"
#include "vf-cpp-entity.h"
#include "vn_introspectionsystem.h"
#include "vs_storageeventsystem.h"
#include "vftestentityspy.h"
#include "vftestcomponentspy.h"
#include "mocklxdmsessionchangeparamgenerator.h"
#include <memory>
#include <unordered_map>

class TestVeinServer : public QObject
{
    Q_OBJECT
public:
    explicit TestVeinServer(const LxdmSessionChangeParam& lxdmParam = MockLxdmSessionChangeParamGenerator::generateTestSessionChanger(true));
    ~TestVeinServer();

    void prependEventSystem(VeinEvent::EventSystem *system);
    void appendEventSystem(VeinEvent::EventSystem *system);
    void simulAllModulesLoaded(const QString &sessionPath, const QStringList &sessionList);
    void changeSession(const QString &sessionPath, int baseEntityId = 20);

    void addTestEntities(int entityCount=2, int componentCount=2, int baseEntityId = 10, int baseComponentNum = 1);
    void addEntity(int entityId, QString entityName);
    void addComponent(int entityId, QString componentName, QVariant initialValue, bool readOnly);
    void removeComponent(int entityId, QString componentName);
    void setComponentClientTransaction(int entityId, QString componentName, QVariant newValue);
    void setComponentServerNotification(int entityId, QString componentName, QVariant newValue);
    QVariant getValue(int entityId, QString componentName);
    void removeEntitiesAdded();
    QMap<int, QList<QString>> getTestEntityComponentInfo();

    VeinEvent::EventHandler* getEventHandler();
    VeinStorage::AbstractEventSystem* getStorage();
    QByteArray dumpStorage(QList<int> entities);
    QByteArray dumpEvents();
    void sendEvent(QEvent *event);

    QList<int> getEntityAddList() const;
    QList<VfTestComponentSpy::TComponentInfo> getComponentAddList() const;
    QList<VfTestComponentSpy::TComponentInfo> getComponentChangeList() const;
    void resetEventSpyData();
private:
    VeinEvent::EventHandler m_vfEventHandler;

    VfTestEntitySpy m_vfEntityAddSpy;
    VfTestComponentSpy m_vfComponentAddSpy;
    VfTestComponentSpy m_vfComponentChangeSpy;
    QJsonObject m_JsonEventLog;
    TestJsonSpyEventSystem m_serverCmdEventSpyTop;
    TestJsonSpyEventSystem m_serverCmdEventSpyBottom;

    SystemModuleEventSystem m_systemModuleSystem;
    VeinNet::IntrospectionSystem m_introspectionSystem;
    VeinStorage::StorageEventSystem m_storageSystem;

    std::unordered_map<int, std::unique_ptr<VfCpp::VfCppEntity>> m_entities;
    QStringList m_sessionList;
};

#endif // TESTVEINSERVER_H
