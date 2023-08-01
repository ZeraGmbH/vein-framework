#ifndef TEST_TASK_CLIENT_COMPONENT_FETCHER_H
#define TEST_TASK_CLIENT_COMPONENT_FETCHER_H

#include <QObject>

class test_task_client_component_fetcher : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void fetchSubscribed();
    void timeout();
private:
    void feedEventLoop();
};

#endif // TEST_TASK_CLIENT_COMPONENT_FETCHER_H
