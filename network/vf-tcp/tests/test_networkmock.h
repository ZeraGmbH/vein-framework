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
    void failPeerNotLocalhost();

    void notStartedServerIsNotListeningReal();
    void notStartedServerIsNotListeningMock();
    void startedServerIsListeningReal();
    void startedServerIsListeningMock();
private:
    void feedEventLoop();
};

#endif // TEST_NETWORKMOCK_H
