#include "vtcp_serverworkermock.h"
#include "vtcp_peer.h"
#include "vtcp_peerworkermock.h"
#include "vtcp_server.h"
#include <systemd/sd-daemon.h>

namespace VeinTcp
{

QHash<quint16 /* port */, TcpServerWorkerMock*> TcpServerWorkerMock::m_serverMocks;

TcpServerWorkerMock::TcpServerWorkerMock(TcpServer *server, secret) :
    m_server(server)
{
}

TcpServerWorkerMock::~TcpServerWorkerMock()
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

bool TcpServerWorkerMock::startServer(quint16 port, bool systemdSocket)
{
    Q_UNUSED(systemdSocket)
    if(!m_serverMocks.contains(port)) {
        m_portListening = port;
        m_serverMocks[port] = this;
        return true;
    }
    return false;
}

bool TcpServerWorkerMock::isListenActive()
{
    return m_portListening != 0;
}

TcpPeerWorkerInterfacePtr TcpServerWorkerMock::createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor)
{
    return std::make_unique<TcpPeerWorkerMock>(peer, socketDescriptor, TcpPeerWorkerMock::secret());
}

TcpServerWorkerMock* TcpServerWorkerMock::getServerMock(quint16 port)
{
    TcpServerWorkerMock *server = nullptr;
    if(m_serverMocks.contains(port))
        server = m_serverMocks[port];
    return server;
}

// called by client peer
TcpPeer* TcpServerWorkerMock::emitSigClientConnected(TcpPeer* clientPeer)
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
            this, &TcpServerWorkerMock::onPeerClosed);
    connect(serverPeer, &TcpPeer::sigConnectionClosed,
            this, &TcpServerWorkerMock::onPeerClosed);

    return serverPeer;
}

void TcpServerWorkerMock::cleanupServerMocks()
{
    m_serverMocks.clear();
}

void TcpServerWorkerMock::onPeerClosed(TcpPeer *peer)
{
    TcpPeer *peerToNotify = nullptr;
    if(m_keyClientValueServer.contains(peer))
        peerToNotify = m_keyClientValueServer[peer];
    else if(m_keyServerValueClient.contains(peer))
        peerToNotify = m_keyServerValueClient[peer];
    if(peerToNotify) {
        removePeersFromHashes(peerToNotify);
        QMetaObject::invokeMethod(peerToNotify,
                                  "sigConnectionClosed",
                                  Qt::QueuedConnection,
                                  Q_ARG(VeinTcp::TcpPeer*, peerToNotify)
                                  );
    }
}

void TcpServerWorkerMock::removePeersFromHashes(TcpPeer *peer)
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
