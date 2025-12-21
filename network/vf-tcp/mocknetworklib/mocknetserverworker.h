#ifndef MOCKNETSERVERWORKER_H
#define MOCKNETSERVERWORKER_H

#include "vtcp_server.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkerinterface.h"
#include <QList>
#include <QHash>

namespace VeinTcp
{
class MockNetServerWorker : public QObject, public TcpServerWorkerInterface
{
    Q_OBJECT
public:
    MockNetServerWorker(TcpServer *server);
    virtual ~MockNetServerWorker();

    bool startServer(quint16 port, bool systemdSocket) override;
    bool isListenActive() override;
    TcpPeerWorkerInterfacePtr createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor) override;
    // for mock peer
    static MockNetServerWorker* getServerMock(quint16 port);
    TcpPeer *emitSigClientConnected(TcpPeer *clientPeer);
private slots:
    void onClientPeerClosed(VeinTcp::TcpPeer *peer);
    void onServerPeerClosed(VeinTcp::TcpPeer *peer);
private:
    void removePeersFromHashes(TcpPeer *peer);
    quint16 m_portListening = 0;
    TcpServer *m_server = nullptr;
    QHash<TcpPeer*, TcpPeer*> m_keyClientValueServer;
    QHash<TcpPeer*, TcpPeer*> m_keyServerValueClient;

    static QHash<quint16 /* port */, MockNetServerWorker*> m_serverMocks;
};
}
#endif // MOCKNETSERVERWORKER_H
