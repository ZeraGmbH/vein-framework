#ifndef TEST_NETWORKMOCK_H
#define TEST_NETWORKMOCK_H

#include <QObject>

class test_networkmock : public QObject
{
    Q_OBJECT
private slots:
    void failPeerConnectNoServerReal();
    void failPeerConnectNoServerMock();

    void failPeerSendNoServerReal();
    void failPeerSendNoServerMock();

    // Mock specific: Mocks simulate local severs only so deny others
    void failPeerNotLocalhostMock();

    void notStartedServerIsNotListeningReal();
    void notStartedServerIsNotListeningMock();

    void startedServerIsListeningReal();
    void startedServerIsListeningMock();

    // we did not get this from reading the code - how stupid we are :(
    void clientConnectServerSideVeryImportantReal();
    void clientConnectServerSideVeryImportantMock();

    void failTwoServersSamePortReal();
    void failTwoServersSamePortMock();

    void clientConnectClientSideEstablishedReal();
    void clientConnectClientSideEstablishedMock();

    // By 'sender' pointer the emitter of signal is meant not the client
    void talkBidirectionalReal();
    void talkBidirectionalMock();

    // Back to default after each
    void cleanup();
};

#endif // TEST_NETWORKMOCK_H
