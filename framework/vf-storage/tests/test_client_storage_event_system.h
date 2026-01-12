#ifndef TEST_CLIENT_STORAGE_EVENT_SYSTEM_H
#define TEST_CLIENT_STORAGE_EVENT_SYSTEM_H

#include "testveinserverwithmocknet.h"
#include "vf_core_stack_client.h"
#include "vs_clientstorageeventsystem.h"
#include <QObject>

class test_client_storage_event_system : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void subscribeToNonExistingEntity();
    void subscribeToExistingEntity();
    void subscribeAndUnsubscribe();
    void subscribeAndEntityRemove();
    void unsubscribeWithoutSubscribing();
    void fetchNonExistingComponent();
    void fetchExistingComponent();
    void serverSetComponent();
    void serverRemoveEntity();
    void serverAddComponent();
    void clientSetComponent();
    void fetchExistingFutureComponent();

    void clientInvokeNonExistingRPC();
    void clientInvokeExistingRpcCorrectParameter();
    void clientInvokeExistingRPCWrongParameter();
    void subscribeToNonExistingEntityRpc();
    void subscribeToExistingEntityWithoutRpc();
    void subscribeToExistingEntityWithRpc();
    void subscribeAndUnsubscribeWithRpc();
    void subscribeAndEntityRemoveWithRpc();

private:
    void setupSpy(QJsonObject &jsonEvents);
    void addEntity(int entityID,
                   const QString &entityName,
                   const QVariantMap &components = QVariantMap());
    void subscribeEntity(int entityID);
    void addAndSubscribeToEntity(int entityID,
                                 const QString &entityName,
                                 const QVariantMap &components = QVariantMap());

    std::unique_ptr<TestVeinServerWithMockNet> m_netServer;
    std::unique_ptr<VfCoreStackClient> m_netClient;
    std::unique_ptr<VeinStorage::ClientStorageEventSystem> m_clientStorageSystem;
    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyBottom;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyBottom;
};

#endif // TEST_CLIENT_STORAGE_EVENT_SYSTEM_H
