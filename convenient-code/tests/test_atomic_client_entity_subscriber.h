#ifndef TEST_ATOMIC_CLIENT_ENTITY_SUBSCRIBER_H
#define TEST_ATOMIC_CLIENT_ENTITY_SUBSCRIBER_H

#include <QObject>

class test_atomic_client_entity_subscriber : public QObject
{
    Q_OBJECT
private slots:
    void intropectSystemEntitySignalReceived();
    void trySubscribeOnNonExistantEntity();
    void introspectComponentNames();
    void invalidIntrospectionData();
private:
    void feedEventLoop();
};

#endif // TEST_ATOMIC_CLIENT_ENTITY_SUBSCRIBER_H
