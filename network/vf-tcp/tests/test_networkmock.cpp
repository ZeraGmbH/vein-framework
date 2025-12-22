#include "test_networkmock.h"
#include "tcpnetworkfactory.h"
#include "mocktcpnetworkfactory.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"
#include <QSignalSpy>
#include <QTest>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_networkmock)

static constexpr int serverPort = 4242;

void test_networkmock::clientConnectKillClientRealFactory()
{
    VeinTcp::TcpServer server(VeinTcp::TcpNetworkFactory::create());
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer *clientPeer = new VeinTcp::TcpPeer(VeinTcp::TcpNetworkFactory::create());
    clientPeer->setPeerId(QUuid::createUuid());
    VeinTcp::TcpPeer *establishedPeer = nullptr;
    QSignalSpy spy(clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished);
    clientPeer->startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    connect(clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished, this, [&](VeinTcp::TcpPeer *peer){
        establishedPeer = peer;
    });

    QVERIFY(spy.wait(1000));
    QCOMPARE(clientPeer->getPeerId(), establishedPeer->getPeerId());
    QCOMPARE(clientPeer, establishedPeer);

    // crasher test
    delete clientPeer;
}

void test_networkmock::clientConnectKillClientMockFactory()
{
    VeinTcp::TcpServer server(VeinTcp::MockTcpNetworkFactory::create());
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer *clientPeer = new VeinTcp::TcpPeer(VeinTcp::MockTcpNetworkFactory::create());
    clientPeer->setPeerId(QUuid::createUuid());
    VeinTcp::TcpPeer *establishedPeer = nullptr;
    QSignalSpy spy(clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished);
    clientPeer->startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    connect(clientPeer, &VeinTcp::TcpPeer::sigConnectionEstablished, this, [&](VeinTcp::TcpPeer *peer){
        establishedPeer = peer;
    });

    QVERIFY(spy.wait(1000));
    QCOMPARE(clientPeer->getPeerId(), establishedPeer->getPeerId());
    QCOMPARE(clientPeer, establishedPeer);

    // crasher test
    delete clientPeer;
}


void test_networkmock::clientConnectKillServerMockFactory()
{
    VeinTcp::TcpServer *server = new VeinTcp::TcpServer(VeinTcp::MockTcpNetworkFactory::create());
    server->startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(VeinTcp::MockTcpNetworkFactory::create());
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

    delete server;

    // check event loop fired: connect after start
    QSignalSpy closeSpy(&clientPeer, &VeinTcp::TcpPeer::sigConnectionClosed);
    QVERIFY(closeSpy.wait(1000));
    QCOMPARE(closeSpy.count(), 1);
}

void test_networkmock::clientConnectKillServerRealFactory()
{
    VeinTcp::TcpServer *server = new VeinTcp::TcpServer(VeinTcp::TcpNetworkFactory::create());
    server->startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(VeinTcp::TcpNetworkFactory::create());
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

    delete server;

    // check event loop fired: connect after start
    QSignalSpy closeSpy(&clientPeer, &VeinTcp::TcpPeer::sigConnectionClosed);
    QVERIFY(closeSpy.wait(1000));
    QCOMPARE(closeSpy.count(), 1);
}

void test_networkmock::failPeerConnectNoServerRealFactory()
{
    VeinTcp::TcpPeer clientPeer(VeinTcp::TcpNetworkFactory::create());
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerConnectNoServerMockFactory()
{
    VeinTcp::TcpPeer clientPeer(VeinTcp::MockTcpNetworkFactory::create());
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerSendNoServerRealFactory()
{
    // howto test? check warning?
    VeinTcp::TcpPeer clientPeer(VeinTcp::TcpNetworkFactory::create());
    clientPeer.sendMessage("foo");
}

void test_networkmock::failPeerSendNoServerMockFactory()
{
    // howto test? check warning?
    VeinTcp::TcpPeer clientPeer(VeinTcp::MockTcpNetworkFactory::create());
    clientPeer.sendMessage("foo");
}

void test_networkmock::failPeerNotLocalhostMockFactory()
{
    VeinTcp::TcpPeer clientPeer(VeinTcp::MockTcpNetworkFactory::create());
    clientPeer.startConnection("192.168.1.1", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::HostNotFoundError));
}

void test_networkmock::notStartedServerIsNotListeningRealFactory()
{
    VeinTcp::TcpServer server(VeinTcp::TcpNetworkFactory::create());
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::notStartedServerIsNotListeningMockFactory()
{
    VeinTcp::TcpServer server(VeinTcp::MockTcpNetworkFactory::create());
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::startedServerIsListeningRealFactory()
{
    VeinTcp::TcpServer server(VeinTcp::TcpNetworkFactory::create());
    server.startServer(serverPort, false);
    // Immediate no event loop!?
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::startedServerIsListeningMockFactory()
{
    VeinTcp::TcpServer server(VeinTcp::MockTcpNetworkFactory::create());
    server.startServer(serverPort, false);
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::clientConnectServerSideVeryImportantRealFactory()
{
    VeinTcp::TcpServer server(VeinTcp::TcpNetworkFactory::create());
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(VeinTcp::TcpNetworkFactory::create());
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

void test_networkmock::clientConnectServerSideVeryImportantMockFactory()
{
    VeinTcp::TcpServer server(VeinTcp::MockTcpNetworkFactory::create());
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(VeinTcp::MockTcpNetworkFactory::create());
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

void test_networkmock::failTwoServersSamePortRealFactory()
{
    VeinTcp::TcpServer server1(VeinTcp::TcpNetworkFactory::create());
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2(VeinTcp::TcpNetworkFactory::create());
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::failTwoServersSamePortMockFactory()
{
    VeinTcp::TcpServer server1(VeinTcp::MockTcpNetworkFactory::create());
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2(VeinTcp::MockTcpNetworkFactory::create());
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::twoServersDifferentPortRealFactory()
{
    VeinTcp::TcpServer server1(VeinTcp::TcpNetworkFactory::create());
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2(VeinTcp::TcpNetworkFactory::create());
    QCOMPARE(server2.startServer(serverPort+1, false), true);
}

void test_networkmock::twoServersDifferentPortMockFactory()
{
    VeinTcp::TcpServer server1(VeinTcp::MockTcpNetworkFactory::create());
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2(VeinTcp::MockTcpNetworkFactory::create());
    QCOMPARE(server2.startServer(serverPort+1, false), true);
}

void test_networkmock::clientConnectClientSideEstablishedRealFactory()
{
    VeinTcp::TcpServer server(VeinTcp::TcpNetworkFactory::create());
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(VeinTcp::TcpNetworkFactory::create());
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

void test_networkmock::clientConnectClientSideEstablishedMockFactory()
{
    VeinTcp::TcpServer server(VeinTcp::MockTcpNetworkFactory::create());
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(VeinTcp::MockTcpNetworkFactory::create());
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

void test_networkmock::talkBidirectionalRealFactory()
{
    VeinTcp::TcpServer server(VeinTcp::TcpNetworkFactory::create());
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(VeinTcp::TcpNetworkFactory::create());
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
    QCOMPARE(spyClientSend[0][1], QByteArray("ClientSend"));

    // server -> client
    // check event loop fired: connect after start
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    serverPeer->sendMessage("ServerSend");
    QVERIFY(spyServerSend.wait(1000));
    QCOMPARE(spyServerSend.count(), 1);
    // Again unexpected!!!
    QCOMPARE(spyServerSend[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend[0][1], QByteArray("ServerSend"));
}

void test_networkmock::talkBidirectionalMockFactory()
{
    VeinTcp::TcpServer server(VeinTcp::MockTcpNetworkFactory::create());
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(VeinTcp::MockTcpNetworkFactory::create());
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
    QCOMPARE(spyClientSend[0][1], QByteArray("ClientSend"));

    // server -> client
    // check event loop fired: connect after start
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    serverPeer->sendMessage("ServerSend");
    QVERIFY(spyServerSend.wait(1000));
    QCOMPARE(spyServerSend.count(), 1);
    // Again unexpected!!!
    QCOMPARE(spyServerSend[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend[0][1], QByteArray("ServerSend"));
}
