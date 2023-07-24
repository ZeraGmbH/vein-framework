#include "test_networkmock.h"
#include "vtcp_workerfactorymethodstest.h"
#include "vtcp_peer.h"
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>
#include <QAbstractEventDispatcher>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_networkmock)

static constexpr int serverPort = 4242;

void test_networkmock::failPeerConnectNoServerReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpPeer clientPeer;
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    clientPeer.startConnection("localhost", serverPort);
    spy.wait(1000);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerConnectNoServerMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpPeer clientPeer;
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    clientPeer.startConnection("localhost", serverPort);
    feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerSendNoServerReal()
{
    // fails with assertion
    /*VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.sendMessage("foo");*/
}

void test_networkmock::failPeerSendNoServerMock()
{
    // fails with assertion
    /*VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.sendMessage("foo");*/
}

void test_networkmock::failPeerNotLocalhostMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpPeer clientPeer;
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    clientPeer.startConnection("192.168.1.1", serverPort);
    feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::HostNotFoundError));
}

void test_networkmock::notStartedServerIsNotListeningReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpServer server;
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::notStartedServerIsNotListeningMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpServer server;
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::startedServerIsListeningReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    // Immediate no event loop!?
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::startedServerIsListeningMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::clientConnectToServerReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer clientPeer;
    clientPeer.startConnection("localhost", serverPort);
    VeinTcp::TcpPeer* peerReceived = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer){
        peerReceived = peer;
    });
    spy.wait(1000);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(peerReceived->getPeerId(), clientPeer.getPeerId());
}

void test_networkmock::clientConnectToServerMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer clientPeer;
    clientPeer.startConnection("localhost", serverPort);
    VeinTcp::TcpPeer* peerReceived = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer){
        peerReceived = peer;
    });
    feedEventLoop();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(peerReceived->getPeerId(), clientPeer.getPeerId());
}

void test_networkmock::failTwoServersSamePortReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpServer server1;
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2;
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::failTwoServersSamePortMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpServer server1;
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2;
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::clientConnectEstablishedReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer;
    clientPeer.setPeerId(QUuid::createUuid());
    VeinTcp::TcpPeer *establishedPeer = nullptr;
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished);
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    connect(&clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished, this, [&](VeinTcp::TcpPeer *peer){
        establishedPeer = peer;
    });
    spy.wait(1000);
    QCOMPARE(clientPeer.getPeerId(), establishedPeer->getPeerId());
}

void test_networkmock::clientConnectEstablishedMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer;
    clientPeer.setPeerId(QUuid::createUuid());
    VeinTcp::TcpPeer *establishedPeer = nullptr;
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished);
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    connect(&clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished, this, [&](VeinTcp::TcpPeer *peer){
        establishedPeer = peer;
    });
    spy.wait(1000);
    QCOMPARE(clientPeer.getPeerId(), establishedPeer->getPeerId());
}


void test_networkmock::feedEventLoop()
{
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
