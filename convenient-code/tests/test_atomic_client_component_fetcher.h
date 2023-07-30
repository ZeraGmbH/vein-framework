#ifndef TEST_ATOMIC_CLIENT_COMPONENT_FETCHER_H
#define TEST_ATOMIC_CLIENT_COMPONENT_FETCHER_H

#include <QObject>

class test_atomic_client_component_fetcher : public QObject
{
    Q_OBJECT
private slots:
    void errorSignalFromUnsubscribedEntityInvalidComponentNoNet();
    void getFromUnsubscribedEntityValidComponentNoNet();
    void noGetFromUnsubscribedEntityValidComponentNet();
    void getFromSubscribedEntityValidComponentNet();
    void getFromSubscribedEntityInvalidComponentNet();
    void getTwoDifferentComponent();
private:
    void feedEventLoop();
};

#endif // TEST_ATOMIC_CLIENT_COMPONENT_FETCHER_H
