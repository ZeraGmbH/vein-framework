#include "test_networkmock.h"
#include "vtcp_workerfactorymethodstest.h"
#include "tcpworkerfactory.h"
#include "mocktcpworkerfactory.h"
#include "vtcp_peer.h"
#include <QSignalSpy>
#include <QTest>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_networkmock)

static constexpr int serverPort = 4242;

void test_networkmock::clientConnectKillClientReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer *clientPeer = new VeinTcp::TcpPeer();
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

void test_networkmock::clientConnectKillClientRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer server(&realFactory);
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer *clientPeer = new VeinTcp::TcpPeer(&realFactory);
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

void test_networkmock::clientConnectKillClientMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer *clientPeer = new VeinTcp::TcpPeer();
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
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer server(&mockFactory);
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer *clientPeer = new VeinTcp::TcpPeer(&mockFactory);
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

void test_networkmock::clientConnectKillServerReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
    VeinTcp::TcpServer *server = new VeinTcp::TcpServer();
    server->startServer(serverPort, false);

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

    delete server;

    // check event loop fired: connect after start
    QSignalSpy closeSpy(&clientPeer, &VeinTcp::TcpPeer::sigConnectionClosed);
    QVERIFY(closeSpy.wait(1000));
    QCOMPARE(closeSpy.count(), 1);
}

void test_networkmock::clientConnectKillServerMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpServer *server = new VeinTcp::TcpServer();
    server->startServer(serverPort, false);

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

    delete server;

    // check event loop fired: connect after start
    QSignalSpy closeSpy(&clientPeer, &VeinTcp::TcpPeer::sigConnectionClosed);
    QVERIFY(closeSpy.wait(1000));
    QCOMPARE(closeSpy.count(), 1);
}

void test_networkmock::clientConnectKillServerMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer *server = new VeinTcp::TcpServer(&mockFactory);
    server->startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(&mockFactory);
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
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer *server = new VeinTcp::TcpServer(&realFactory);
    server->startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(&realFactory);
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

void test_networkmock::failPeerConnectNoServerReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerConnectNoServerRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpPeer clientPeer(&realFactory);
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerConnectNoServerMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerConnectNoServerMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpPeer clientPeer(&mockFactory);
    clientPeer.startConnection("localhost", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::ConnectionRefusedError));
}

void test_networkmock::failPeerSendNoServerReal()
{
    // howto test? check warning?
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.sendMessage("foo");
}

void test_networkmock::failPeerSendNoServerRealFactory()
{
    // howto test? check warning?
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpPeer clientPeer(&realFactory);
    clientPeer.sendMessage("foo");
}

void test_networkmock::failPeerSendNoServerMock()
{
    // howto test? check warning?
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.sendMessage("foo");
}

void test_networkmock::failPeerSendNoServerMockFactory()
{
    // howto test? check warning?
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpPeer clientPeer(&mockFactory);
    clientPeer.sendMessage("foo");
}

void test_networkmock::failPeerNotLocalhostMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpPeer clientPeer;
    clientPeer.startConnection("192.168.1.1", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::HostNotFoundError));
}

void test_networkmock::failPeerNotLocalhostMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpPeer clientPeer(&mockFactory);
    clientPeer.startConnection("192.168.1.1", serverPort);
    // check event loop fired: connect after start
    QSignalSpy spy(&clientPeer, &VeinTcp::TcpPeer::sigSocketError);
    QVERIFY(spy.wait(1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy[0][1], QVariant(QAbstractSocket::HostNotFoundError));
}

void test_networkmock::notStartedServerIsNotListeningReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
    VeinTcp::TcpServer server;
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::notStartedServerIsNotListeningRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer server(&realFactory);
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::notStartedServerIsNotListeningMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpServer server;
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::notStartedServerIsNotListeningMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer server(&mockFactory);
    QCOMPARE(server.isListening(), false);
}

void test_networkmock::startedServerIsListeningReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    // Immediate no event loop!?
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::startedServerIsListeningRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer server(&realFactory);
    server.startServer(serverPort, false);
    // Immediate no event loop!?
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::startedServerIsListeningMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpServer server;
    server.startServer(serverPort, false);
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::startedServerIsListeningMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer server(&mockFactory);
    server.startServer(serverPort, false);
    QCOMPARE(server.isListening(), true);
}

void test_networkmock::clientConnectServerSideVeryImportantReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
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

void test_networkmock::clientConnectServerSideVeryImportantRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer server(&realFactory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(&realFactory);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
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

void test_networkmock::clientConnectServerSideVeryImportantMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer server(&mockFactory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(&mockFactory);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
    VeinTcp::TcpServer server1;
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2;
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::failTwoServersSamePortRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer server1(&realFactory);
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2(&realFactory);
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::failTwoServersSamePortMock()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
    VeinTcp::TcpServer server1;
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2;
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::failTwoServersSamePortMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer server1(&mockFactory);
    QCOMPARE(server1.startServer(serverPort, false), true);
    VeinTcp::TcpServer server2(&mockFactory);
    QCOMPARE(server2.startServer(serverPort, false), false);
}

void test_networkmock::clientConnectClientSideEstablishedReal()
{
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
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

void test_networkmock::clientConnectClientSideEstablishedRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer server(&realFactory);
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(&realFactory);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
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

void test_networkmock::clientConnectClientSideEstablishedMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer server(&mockFactory);
    server.startServer(serverPort, false);

    VeinTcp::TcpPeer clientPeer(&mockFactory);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
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

void test_networkmock::talkBidirectionalRealFactory()
{
    VeinTcp::TcpWorkerFactory realFactory;
    VeinTcp::TcpServer server(&realFactory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(&realFactory);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableMockNetwork();
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

void test_networkmock::talkBidirectionalMockFactory()
{
    VeinTcp::MockTcpWorkerFactory mockFactory;
    VeinTcp::TcpServer server(&mockFactory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(&mockFactory);
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
    VeinTcp::TcpWorkerFactoryMethodsTest::enableRealNetwork();
}
