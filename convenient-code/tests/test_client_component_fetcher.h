#ifndef TEST_ATOMIC_CLIENT_COMPONENT_FETCHER_H
#define TEST_ATOMIC_CLIENT_COMPONENT_FETCHER_H

#include <QObject>

class test_client_component_fetcher : public QObject
{
    Q_OBJECT
private slots:
    void errorSignalFromUnsubscribedEntityInvalidComponentNoNet();
    void getFromUnsubscribedEntityValidComponentNoNet();
    void noGetFromUnsubscribedEntityValidComponentNet();
    void getFromSubscribedEntityValidComponentNet();
    void getFromSubscribedEntityNonExistentComponentNet();
    void getFromSubscribedEntityInvalidComponentNet();
    void getTwoDifferentComponent();
};

#endif // TEST_ATOMIC_CLIENT_COMPONENT_FETCHER_H
