#ifndef TEST_CLIENT_COMMAND_EVENTS_H
#define TEST_CLIENT_COMMAND_EVENTS_H

#include "testveinserverwithmocknet.h"
#include "vf_core_stack_client.h"
#include "vf_entity_component_event_item.h"
#include "testjsonspyeventsystem.h"
#include <memory>

class test_command_events : public QObject
{
    Q_OBJECT
private slots:
    void clientSubscribeEntity();
    void clientSubscribeNonExistingEntity();
    void clientSubscribeUnsubscribeEntity();
    void clientFetchComponent();
    void clientFetchNonExistingComponent();
    void clientSetSytemEnttityComponent();
    void clientSetNonExistingComponent();

    void serverAddEntityAndComponent();
    void serverAddComponentWithSubscribedClient();
    void serverAddComponentForNomExistentEntity();
    void serverRemoveComponent();
    void serverRemoveNonExistingComponent();
    void serverRemoveComponentForNomExistentEntity();
    void serverRemoveAllEntities();

    void init();
    void cleanup();
private:
    void setupSpy(QJsonObject &jsonEvents);
    void subscribeClient(int entityId);
    std::unique_ptr<TestVeinServerWithMockNet> m_netServer;
    std::unique_ptr<VfCoreStackClient> m_netClient;
    VfCmdEventItemEntityPtr m_entityItem;
    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_clientCmdEventSpyBottom;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyTop;
    std::unique_ptr<TestJsonSpyEventSystem> m_serverCmdEventSpyBottom;
};

#endif // TEST_CLIENT_COMMAND_EVENTS_H
