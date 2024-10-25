#ifndef TEST_NETWORKMOCK_H
#define TEST_NETWORKMOCK_H

#include <QObject>

class test_networkmock : public QObject
{
    Q_OBJECT
private slots:

    void clientConnectKillClientReal();
    void clientConnectKillClientRealFactory();
    void clientConnectKillClientMock();
    void clientConnectKillClientMockFactory();

    void clientConnectKillServerReal();
    void clientConnectKillServerRealFactory();
    void clientConnectKillServerMock();
    void clientConnectKillServerMockFactory();

    void failPeerConnectNoServerReal();
    void failPeerConnectNoServerRealFactory();
    void failPeerConnectNoServerMock();
    void failPeerConnectNoServerMockFactory();

    void failPeerSendNoServerReal();
    void failPeerSendNoServerRealFactory();
    void failPeerSendNoServerMock();
    void failPeerSendNoServerMockFactory();

    // Mock specific: Mocks simulate local severs only so deny others
    void failPeerNotLocalhostMock();
    void failPeerNotLocalhostMockFactory();

    void notStartedServerIsNotListeningReal();
    void notStartedServerIsNotListeningRealFactory();
    void notStartedServerIsNotListeningMock();
    void notStartedServerIsNotListeningMockFactory();

    void startedServerIsListeningReal();
    void startedServerIsListeningRealFactory();
    void startedServerIsListeningMock();
    void startedServerIsListeningMockFactory();

    // we did not get this from reading the code - how stupid we are :(
    void clientConnectServerSideVeryImportantReal();
    void clientConnectServerSideVeryImportantRealFactory();
    void clientConnectServerSideVeryImportantMock();
    void clientConnectServerSideVeryImportantMockFactory();

    void failTwoServersSamePortReal();
    void failTwoServersSamePortRealFactory();
    void failTwoServersSamePortMock();
    void failTwoServersSamePortMockFactory();

    void twoServersDifferentPortRealFactory();
    void twoServersDifferentPortMockFactory();

    void clientConnectClientSideEstablishedReal();
    void clientConnectClientSideEstablishedRealFactory();
    void clientConnectClientSideEstablishedMock();
    void clientConnectClientSideEstablishedMockFactory();

    // By 'sender' pointer the emitter of signal is meant not the client
    void talkBidirectionalReal();
    void talkBidirectionalRealFactory();
    void talkBidirectionalMock();
    void talkBidirectionalMockFactory();

    // Back to default after each
    void cleanup();
};

#endif // TEST_NETWORKMOCK_H
