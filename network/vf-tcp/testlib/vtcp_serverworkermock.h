#ifndef VEIN_TCP_SERVERWORKERMOCK_H
#define VEIN_TCP_SERVERWORKERMOCK_H

#include "vtcp_server.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkerinterface.h"
#include <QList>

namespace VeinTcp
{
class TcpServerWorkerMock : public QObject, public TcpServerWorkerInterface
{
    Q_OBJECT
private:
    // Just our friend TcpWorkerFactoryMethodsTest can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    struct secret { explicit secret() = default; };
    friend class TcpWorkerFactoryMethodsTest;
public:
    TcpServerWorkerMock(TcpServer *server, secret);
    virtual ~TcpServerWorkerMock();

    bool startServer(quint16 port, bool systemdSocket) override;
    bool isListenActive() override;
    TcpPeerWorkerInterfacePtr createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor) override;
    // for mock peer
    static TcpServerWorkerMock* getServerMock(quint16 port);
    TcpPeer *emitSigClientConnected(TcpPeer *clientPeer);
    // for TcpWorkerFactoryMethodsTest
    static void cleanupServerMocks();
private slots:
    void onPeerClosed(VeinTcp::TcpPeer *peer);
private:
    void removePeersFromHashes(TcpPeer *peer);
    quint16 m_portListening = 0;
    TcpServer *m_server = nullptr;
    QHash<TcpPeer*, TcpPeer*> m_keyClientValueServer;
    QHash<TcpPeer*, TcpPeer*> m_keyServerValueClient;

    static QHash<quint16 /* port */, TcpServerWorkerMock*> m_serverMocks;
};
}
#endif // VEIN_TCP_SERVERWORKERMOCK_H
