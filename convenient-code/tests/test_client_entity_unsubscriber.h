#ifndef TEST_ATOMIC_CLIENT_ENTITY_UNSUBSCRIBER_H
#define TEST_ATOMIC_CLIENT_ENTITY_UNSUBSCRIBER_H

#include <QObject>

class test_client_entity_unsubscriber : public QObject
{
    Q_OBJECT
private slots:
    void unsubscribeOnNotSubscribed();
    void subscribeUnsubscribe();
};

#endif // TEST_ATOMIC_CLIENT_ENTITY_UNSUBSCRIBER_H
