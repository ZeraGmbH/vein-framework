#include "mocknetserverworker.h"
#include "mocknetpeerworker.h"
#include "vtcp_peer.h"
#include "vtcp_server.h"
#include <systemd/sd-daemon.h>

namespace VeinTcp
{

QHash<quint16 /* port */, MockNetServerWorker*> MockNetServerWorker::m_serverMocks;

MockNetServerWorker::MockNetServerWorker(TcpServer *server) :
    m_server(server)
{
}

MockNetServerWorker::~MockNetServerWorker()
{
    m_serverMocks.remove(m_portListening);

    QList<TcpPeer*> peersToRemove;
    for(auto iter=m_keyClientValueServer.constBegin(); iter!=m_keyClientValueServer.constEnd(); iter++) {
        peersToRemove.append(iter.key());
        peersToRemove.append(iter.value());
    }
    for(auto peer : qAsConst(peersToRemove))
        QMetaObject::invokeMethod(peer,
                                  "sigConnectionClosed",
                                  Qt::QueuedConnection,
                                  Q_ARG(VeinTcp::TcpPeer*, peer)
                                  );
}

bool MockNetServerWorker::startServer(quint16 port, bool systemdSocket)
{
    Q_UNUSED(systemdSocket)
    if(!m_serverMocks.contains(port)) {
        m_portListening = port;
        m_serverMocks[port] = this;
        return true;
    }
    return false;
}

bool MockNetServerWorker::isListenActive()
{
    return m_portListening != 0;
}

TcpPeerWorkerInterfacePtr MockNetServerWorker::createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor)
{
    return std::make_unique<MockNetPeerWorker>(peer, socketDescriptor);
}

MockNetServerWorker* MockNetServerWorker::getServerMock(quint16 port)
{
    MockNetServerWorker *server = nullptr;
    if(m_serverMocks.contains(port))
        server = m_serverMocks[port];
    return server;
}

// called by client peer
TcpPeer* MockNetServerWorker::emitSigClientConnected(TcpPeer* clientPeer)
{
    TcpPeer* serverPeer = new VeinTcp::TcpPeer(qintptr(clientPeer), this);
    QMetaObject::invokeMethod(m_server,
                              "sigClientConnected",
                              Qt::QueuedConnection,
                              Q_ARG(VeinTcp::TcpPeer*, serverPeer));
    Q_ASSERT(!m_keyClientValueServer.contains(clientPeer));
    m_keyClientValueServer[clientPeer] = serverPeer;
    Q_ASSERT(!m_keyServerValueClient.contains(serverPeer));
    m_keyServerValueClient[serverPeer] = clientPeer;

    connect(clientPeer, &TcpPeer::sigConnectionClosed,
            this, &MockNetServerWorker::onClientPeerClosed);
    connect(serverPeer, &TcpPeer::sigConnectionClosed,
            this, &MockNetServerWorker::onServerPeerClosed);

    return serverPeer;
}

void MockNetServerWorker::onClientPeerClosed(TcpPeer *peer)
{
    TcpPeer *peerToNotify = nullptr;
    if(m_keyClientValueServer.contains(peer))
        peerToNotify = m_keyClientValueServer[peer];
    if(peerToNotify) {
        removePeersFromHashes(peerToNotify);
        QMetaObject::invokeMethod(peerToNotify,
                                  "sigConnectionClosed",
                                  Qt::QueuedConnection,
                                  Q_ARG(VeinTcp::TcpPeer*, peerToNotify)
                                  );
    }
}

void MockNetServerWorker::onServerPeerClosed(TcpPeer *peer)
{
    TcpPeer *peerToNotify = nullptr;
    if(m_keyServerValueClient.contains(peer))
        peerToNotify = m_keyServerValueClient[peer];
    if(peerToNotify) {
        removePeersFromHashes(peerToNotify);
        QMetaObject::invokeMethod(peerToNotify,
                                  "sigConnectionClosed",
                                  Qt::QueuedConnection,
                                  Q_ARG(VeinTcp::TcpPeer*, peerToNotify)
                                  );
    }
    // note that client/server notation is different to production code:
    // * Production code just handles peers created on connect and calls them clients
    //   and does cleanup for them - see TcpServerWorker::clientDisconnectedSRV
    // * Here we have access to peers created by client and leave housekeeping to clients /
    //   the peers generated at connection are server peers which we have to delete:
    peer->deleteLater();
}

void MockNetServerWorker::removePeersFromHashes(TcpPeer *peer)
{
    if(m_keyClientValueServer.contains(peer)) {
        TcpPeer* serverPeer = m_keyClientValueServer.take(peer);
        m_keyServerValueClient.remove(serverPeer);
    }
    if(m_keyServerValueClient.contains(peer)) {
        TcpPeer* clientPeer = m_keyServerValueClient.take(peer);
        m_keyClientValueServer.remove(clientPeer);
    }
}

}
