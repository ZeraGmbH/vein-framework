#include "test_networkmock.h"
#include "vtcp_workerfactorymethodstest.h"
#include "vtcp_peer.h"
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
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerConnectNoServerMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
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
    clientPeer.startConnection("192.168.1.1", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
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

void test_networkmock::clientConnectServerSideVeryImportantReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer;
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer){
        serverPeer = peer;
    });
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QVERIFY(&clientPeer != serverPeer);
    // server peer has no uuid (odd - seems upon higher layers)
    QVERIFY(serverPeer->getPeerId().isNull());
}

void test_networkmock::clientConnectServerSideVeryImportantMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer;
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer){
        serverPeer = peer;
    });
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QVERIFY(&clientPeer != serverPeer);
    // server peer has no uuid (odd - seems upon higher layers)
    QVERIFY(serverPeer->getPeerId().isNull());
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

void test_networkmock::clientConnectClientSideEstablishedReal()
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
    QVERIFY(spy.wait(1000));
    QCOMPARE(clientPeer.getPeerId(), establishedPeer->getPeerId());
    QCOMPARE(&clientPeer, establishedPeer);
}

void test_networkmock::clientConnectClientSideEstablishedMock()
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
    QVERIFY(spy.wait(1000));
    QCOMPARE(clientPeer.getPeerId(), establishedPeer->getPeerId());
    QCOMPARE(&clientPeer, establishedPeer);
}

void test_networkmock::talkBidirectionalReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer;
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    QVERIFY(spy.wait(1000));

    // client -> server
    // check event loop fired: connect after start
    QSignalSpy spyClientSend(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    clientPeer.sendMessage("ClientSend");
    QVERIFY(spyClientSend.wait(1000));
    QCOMPARE(spyClientSend.count(), 1);
    // Again unexpected!!!
    QCOMPARE(spyClientSend[0][0].value<VeinTcp::TcpPeer*>(), serverPeer);
    QCOMPARE(spyClientSend[0][1], "ClientSend");

    // server -> client
    // check event loop fired: connect after start
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    serverPeer->sendMessage("ServerSend");
    QVERIFY(spyServerSend.wait(1000));
    QCOMPARE(spyServerSend.count(), 1);
    // Again unexpected!!!
    QCOMPARE(spyServerSend[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend[0][1], "ServerSend");

}

void test_networkmock::talkBidirectionalMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMock();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer;
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    QVERIFY(spy.wait(1000));

    // client -> server
    // check event loop fired: connect after start
    QSignalSpy spyClientSend(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    clientPeer.sendMessage("ClientSend");
    QVERIFY(spyClientSend.wait(1000));
    QCOMPARE(spyClientSend.count(), 1);
    // Again unexpected!!!
    QCOMPARE(spyClientSend[0][0].value<VeinTcp::TcpPeer*>(), serverPeer);
    QCOMPARE(spyClientSend[0][1], "ClientSend");

    // server -> client
    // check event loop fired: connect after start
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    serverPeer->sendMessage("ServerSend");
    QVERIFY(spyServerSend.wait(1000));
    QCOMPARE(spyServerSend.count(), 1);
    // Again unexpected!!!
    QCOMPARE(spyServerSend[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend[0][1], "ServerSend");
}

void test_networkmock::cleanup()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableProduction();
}
