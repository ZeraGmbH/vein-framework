#ifndef TEST_CLIENT_COMMAND_EVENTS_H
#define TEST_CLIENT_COMMAND_EVENTS_H

#include <QObject>

class test_client_command_events : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void subscribeSystemEntity();
    void fetchSystemEntity();
};

#endif // TEST_CLIENT_COMMAND_EVENTS_H
