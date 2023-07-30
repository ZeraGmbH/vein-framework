#ifndef TEST_COMMAND_EVENT_HANDLER_H
#define TEST_COMMAND_EVENT_HANDLER_H

#include <QObject>

class test_command_event_handler : public QObject
{
    Q_OBJECT
private slots:
    void notificationFilter();
    void transactionFilter();
    void multipleEntities();
    void multipleItems();
    void unknownEventType();
    void handleErrorEvents();
};

#endif // TEST_COMMAND_EVENT_HANDLER_H
