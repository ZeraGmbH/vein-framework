#ifndef TEST_TASK_CLIENT_ENTITY_SUBSCRIBE_H
#define TEST_TASK_CLIENT_ENTITY_SUBSCRIBE_H

#include <QObject>

class test_task_client_entity_subscribe : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void subscibeOk();
    void timeout();
    void invalidEntity();
};

#endif // TEST_TASK_CLIENT_ENTITY_SUBSCRIBE_H
