#ifndef TEST_TASK_CLIENT_COMPONENT_SETTER_H
#define TEST_TASK_CLIENT_COMPONENT_SETTER_H

#include <QObject>

class test_task_client_component_setter : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void setValidValueSubscribed();
    void timeout();
private:
    void feedEventLoop();
};

#endif // TEST_TASK_CLIENT_COMPONENT_SETTER_H
