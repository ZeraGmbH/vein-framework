#ifndef TEST_NETWORKMOCK_H
#define TEST_NETWORKMOCK_H

#include <QObject>

class test_networkmock : public QObject
{
    Q_OBJECT
private slots:

    void clientConnectKillClientRealFactory();
    void clientConnectKillClientMockFactory();

    void clientConnectKillServerRealFactory();
    void clientConnectKillServerMockFactory();

    void failPeerConnectNoServerRealFactory();
    void failPeerConnectNoServerMockFactory();

    void failPeerSendNoServerRealFactory();
    void failPeerSendNoServerMockFactory();

    // Mock specific: Mocks simulate local severs only so deny others
    void failPeerNotLocalhostMockFactory();

    void notStartedServerIsNotListeningRealFactory();
    void notStartedServerIsNotListeningMockFactory();

    void startedServerIsListeningRealFactory();
    void startedServerIsListeningMockFactory();

    // we did not get this from reading the code - how stupid we are :(
    void clientConnectServerSideVeryImportantRealFactory();
    void clientConnectServerSideVeryImportantMockFactory();

    void failTwoServersSamePortRealFactory();
    void failTwoServersSamePortMockFactory();

    void twoServersDifferentPortRealFactory();
    void twoServersDifferentPortMockFactory();

    void clientConnectClientSideEstablishedRealFactory();
    void clientConnectClientSideEstablishedMockFactory();

    // By 'sender' pointer the emitter of signal is meant not the client
    void talkBidirectionalRealFactory();
    void talkBidirectionalMockFactory();

    // Back to default after each
    void cleanup();
};

#endif // TEST_NETWORKMOCK_H
