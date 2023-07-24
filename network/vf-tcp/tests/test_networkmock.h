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
};

#endif // TEST_NETWORKMOCK_H
