#ifndef TEST_CLIENT_COMMAND_EVENTS_H
#define TEST_CLIENT_COMMAND_EVENTS_H

#include "testveinserverwithnet.h"
#include "vf_core_stack_client.h"
#include "vf_entity_component_event_item.h"
#include "testcommandeventspyeventsystem.h"
#include <memory>

class test_command_events : public QObject
{
    Q_OBJECT
private slots:
    void subscribeSystemEntity();
    void subscribeNonExistingEntity();
    void fetchSystemEntityComponent();
    void fetchSystemEntityNonExistingComponent();
    void setSystemEntityComponent();

    void serverAddEntityAndComponent();

    void initTestCase();
    void init();
    void cleanup();
private:
    void setupSpy(QJsonObject &jsonEvents);
    void subscribeClient(int entityId);
    std::unique_ptr<TestVeinServerWithNet> m_netServer;
    std::unique_ptr<VfCoreStackClient> m_netClient;
    VfCmdEventItemEntityPtr m_entityItem;
    std::unique_ptr<TestCommandEventSpyEventSystem> m_clientCmdEventSpy;
    std::unique_ptr<TestCommandEventSpyEventSystem> m_serverCmdEventSpy;
};

#endif // TEST_CLIENT_COMMAND_EVENTS_H
