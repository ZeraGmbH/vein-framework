#ifndef TEST_CLIENT_COMMAND_EVENTS_H
#define TEST_CLIENT_COMMAND_EVENTS_H

#include "testveinserverwithnet.h"
#include "vf_core_stack_client.h"
#include "vf_entity_component_event_item.h"
#include <memory>

class test_client_command_events : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void cleanup();
    void subscribeSystemEntity();
    void fetchSystemEntityComponent();
private:
    void subscribeClient(int entityId);
    std::unique_ptr<TestVeinServerWithNet> m_netServer;
    std::unique_ptr<VfCoreStackClient> m_netClient;
    VfCmdEventItemEntityPtr m_entityItem;
};

#endif // TEST_CLIENT_COMMAND_EVENTS_H
