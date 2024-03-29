#ifndef TEST_CLIENT_COMMAND_EVENTS_H
#define TEST_CLIENT_COMMAND_EVENTS_H

#include "testveinserverwithnet.h"
#include <memory>

class test_client_command_events : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void cleanup();
    void subscribeSystemEntity();
    void fetchSystemEntity();
private:
    std::unique_ptr<TestVeinServerWithNet> m_netServer;
};

#endif // TEST_CLIENT_COMMAND_EVENTS_H
