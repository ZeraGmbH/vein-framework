#include "test_network_fragmented_transfer.h"
#include "vtcp_server.h"
#include "testfragmentedtcpnetworkfactory.h"
#include "testfragmentednetpeerworker.h"
#include <signalspywaiter.h>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(test_network_fragmented_transfer)

static constexpr int serverPort = 4242;
static constexpr int netTimeout = 1000;
static constexpr int streamDataLen = 4;
static const char* serverSendStr = "ServerSend";
static const char* clientSendStr = "ClientSend";

void test_network_fragmented_transfer::initTestCase_data()
{
    QTest::addColumn<int>("fragmentedSize");

    QTest::newRow("Unfragmented") << 0;
    QTest::newRow("Fragmented Size 1") << 1;
    QTest::newRow("Fragmented Size 2") << 2;
    QTest::newRow("Fragmented Size 3") << 3;
    QTest::newRow("Fragmented Size 4") << 4;
    QTest::newRow("Fragmented Size 8") << 8;
    QTest::newRow("Fragmented Size 16") << 16;
    QTest::newRow("Fragmented Size 32") << 32;
    QTest::newRow("Fragmented Size 64") << 64;
    QTest::newRow("Fragmented Size 128") << 128;
}

void test_network_fragmented_transfer::init()
{
    QFETCH_GLOBAL(int, fragmentedSize);
    m_fragmentedSize = fragmentedSize;
    VeinTcp::TestFragmentedNetPeerWorker::resetReadyReadCount();
}

void test_network_fragmented_transfer::testTestPeerWorkerNotFragmented()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(0, 0);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // client -> server
    QSignalSpy spyClientSend(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send1 = QByteArray(clientSendStr);
    clientPeer.sendMessage(send1);
    SignalSpyWaiter::waitForSignals(&spyClientSend, 1, netTimeout);
    QCOMPARE(spyClientSend.count(), 1);
    QCOMPARE(spyClientSend[0][1], send1);

    // server -> client
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send2 = QByteArray(serverSendStr);
    serverPeer->sendMessage(send2);
    SignalSpyWaiter::waitForSignals(&spyServerSend, 1, netTimeout);
    QCOMPARE(spyServerSend.count(), 1);
    QCOMPARE(spyServerSend[0][1], send2);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(), 2);
}

void test_network_fragmented_transfer::testTestPeerWorkerHugeFragment()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(5000, 5000);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // client -> server
    QSignalSpy spyClientSend(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send1 = QByteArray(clientSendStr);
    clientPeer.sendMessage(send1);
    SignalSpyWaiter::waitForSignals(&spyClientSend, 1, netTimeout);
    QCOMPARE(spyClientSend.count(), 1);
    QCOMPARE(spyClientSend[0][1], send1);

    // server -> client
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send2 = QByteArray(serverSendStr);
    serverPeer->sendMessage(send2);
    SignalSpyWaiter::waitForSignals(&spyServerSend, 1, netTimeout);
    QCOMPARE(spyServerSend.count(), 1);
    QCOMPARE(spyServerSend[0][1], send2);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(), 2);
}

void test_network_fragmented_transfer::sendServerOneMsgFragmented()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(m_fragmentedSize, 0);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // server -> client
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send = QByteArray(serverSendStr);
    serverPeer->sendMessage(send);
    SignalSpyWaiter::waitForSignals(&spyServerSend, 1, netTimeout);
    QCOMPARE(spyServerSend.count(), 1);
    QCOMPARE(spyServerSend[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend[0][1], send);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(),
             calcTransactionCount(SendList() << send));
}

void test_network_fragmented_transfer::sendServerTwoMsgFragmentedOneTransaction()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(m_fragmentedSize, 0);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // 2 * server -> client
    QSignalSpy spyServerSend(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send = QByteArray(serverSendStr);
    serverPeer->sendMessage(send);
    serverPeer->sendMessage(send);
    SignalSpyWaiter::waitForSignals(&spyServerSend, 2, netTimeout);
    QCOMPARE(spyServerSend.count(), 2);
    QCOMPARE(spyServerSend[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend[0][1], send);
    QCOMPARE(spyServerSend[1][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend[1][1], send);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(),
             calcTransactionCount(SendList() << send << send));
}

void test_network_fragmented_transfer::sendServerTwoMsgFragmentedTwoTransactions()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(m_fragmentedSize, 0);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // server -> client 1
    QSignalSpy spyServerSend1(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send1 = QByteArray(serverSendStr) + "1";
    serverPeer->sendMessage(send1);
    SignalSpyWaiter::waitForSignals(&spyServerSend1, 1, netTimeout);
    QCOMPARE(spyServerSend1.count(), 1);
    QCOMPARE(spyServerSend1[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend1[0][1], send1);

    // server -> client 2
    QSignalSpy spyServerSend2(&clientPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send2 = QByteArray(serverSendStr) + "2";
    serverPeer->sendMessage(send2);
    SignalSpyWaiter::waitForSignals(&spyServerSend2, 1, netTimeout);
    QCOMPARE(spyServerSend2.count(), 1);
    QCOMPARE(spyServerSend2[0][0].value<VeinTcp::TcpPeer*>(), &clientPeer);
    QCOMPARE(spyServerSend2[0][1], send2);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(),
             calcTransactionCount(SendList() << send1) + calcTransactionCount(SendList()<< send2));
}

void test_network_fragmented_transfer::sendClientOneMsgFragmented()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(0, m_fragmentedSize);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // client -> server
    QSignalSpy spyClientSend(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send = QByteArray(clientSendStr);
    clientPeer.sendMessage(send);
    SignalSpyWaiter::waitForSignals(&spyClientSend, 1, netTimeout);
    QCOMPARE(spyClientSend.count(), 1);
    QCOMPARE(spyClientSend[0][0].value<VeinTcp::TcpPeer*>(), serverPeer);
    QCOMPARE(spyClientSend[0][1], send);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(),
             calcTransactionCount(SendList() << send));
}

void test_network_fragmented_transfer::sendClientTwoMsgFragmentedOneTransaction()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(0, m_fragmentedSize);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // client -> server
    QSignalSpy spyClientSend(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send = QByteArray(clientSendStr);
    clientPeer.sendMessage(send);
    clientPeer.sendMessage(send);
    SignalSpyWaiter::waitForSignals(&spyClientSend, 2, netTimeout);
    QCOMPARE(spyClientSend.count(), 2);
    QCOMPARE(spyClientSend[0][0].value<VeinTcp::TcpPeer*>(), serverPeer);
    QCOMPARE(spyClientSend[0][1], send);
    QCOMPARE(spyClientSend[1][0].value<VeinTcp::TcpPeer*>(), serverPeer);
    QCOMPARE(spyClientSend[1][1], send);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(),
             calcTransactionCount(SendList() << send << send));
}

void test_network_fragmented_transfer::sendClientTwoMsgFragmentedTwoTransactions()
{
    VeinTcp::AbstractTcpNetworkFactoryPtr factory =
        VeinTcp::TestFragmentedTcpNetworkFactory::create(0, m_fragmentedSize);
    VeinTcp::TcpServer server(factory);
    server.startServer(serverPort, false);
    VeinTcp::TcpPeer clientPeer(factory);
    clientPeer.setPeerId(QUuid::createUuid());
    clientPeer.startConnection("localhost", serverPort);

    QSignalSpy spy(&server, &VeinTcp::TcpServer::sigClientConnected);
    VeinTcp::TcpPeer* serverPeer = nullptr;
    connect(&server, &VeinTcp::TcpServer::sigClientConnected, this, [&](VeinTcp::TcpPeer *peer) {
        serverPeer = peer;
    });
    SignalSpyWaiter::waitForSignals(&spy, 1, netTimeout);

    // client -> server 1
    QSignalSpy spyClientSend1(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send1 = QByteArray(clientSendStr) + "1";
    clientPeer.sendMessage(send1);
    SignalSpyWaiter::waitForSignals(&spyClientSend1, 1, netTimeout);
    QCOMPARE(spyClientSend1.count(), 1);
    QCOMPARE(spyClientSend1[0][0].value<VeinTcp::TcpPeer*>(), serverPeer);
    QCOMPARE(spyClientSend1[0][1], send1);

    // client -> server 2
    QSignalSpy spyClientSend2(serverPeer, &VeinTcp::TcpPeer::sigMessageReceived);
    const QByteArray send2 = QByteArray(clientSendStr) + "2";
    clientPeer.sendMessage(send2);
    SignalSpyWaiter::waitForSignals(&spyClientSend2, 1, netTimeout);
    QCOMPARE(spyClientSend2.count(), 1);
    QCOMPARE(spyClientSend2[0][0].value<VeinTcp::TcpPeer*>(), serverPeer);
    QCOMPARE(spyClientSend2[0][1], send2);

    QCOMPARE(VeinTcp::TestFragmentedNetPeerWorker::getReadyReadCount(),
             calcTransactionCount(SendList() << send1) + calcTransactionCount(SendList()<< send2));
}

int test_network_fragmented_transfer::calcTransactionCount(const SendList &transferredStrings)
{
    int transferredBytes = 0;
    for (const QByteArray &send : transferredStrings)
        transferredBytes += send.size();
    transferredBytes += streamDataLen * transferredStrings.count();

    if (m_fragmentedSize == 0 || m_fragmentedSize >= transferredBytes)
        return 1;
    int transactionCount = transferredBytes / m_fragmentedSize;
    if (transferredBytes % m_fragmentedSize)
        transactionCount++;
    return transactionCount;
}
