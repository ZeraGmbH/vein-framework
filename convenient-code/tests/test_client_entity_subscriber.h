#ifndef TEST_ATOMIC_CLIENT_ENTITY_SUBSCRIBER_H
#define TEST_ATOMIC_CLIENT_ENTITY_SUBSCRIBER_H

#include <QObject>

class test_client_entity_subscriber : public QObject
{
    Q_OBJECT
private slots:
    void intropectSystemEntitySignalReceived();
    void intropectSystemEntitySignalReceivedNetwork();
    void intropectSystemEntityTwiceNetwork();
    void trySubscribeOnNonExistantEntity();
    void trySubscribeOnNonExistantEntityTogetherwithOtherError();
    void introspectComponentNames();
    void introspectComponentContents();
    void invalidIntrospectionData();
};

#endif // TEST_ATOMIC_CLIENT_ENTITY_SUBSCRIBER_H
