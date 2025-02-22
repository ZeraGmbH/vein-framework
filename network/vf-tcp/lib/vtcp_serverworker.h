#ifndef VEIN_TCP_SERVERPRIVATE_H
#define VEIN_TCP_SERVERPRIVATE_H

#include "vtcp_server.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkerinterface.h"
#include <QList>
#include <QTcpServer>

namespace VeinTcp
{
class TcpServerWorker : public QTcpServer, public TcpServerWorkerInterface
{
    Q_OBJECT
private:
    // Just our friends can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    struct secret { explicit secret() = default; };
    friend class TcpNetworkFactory;
public:
    TcpServerWorker(TcpServer *server, secret);
    ~TcpServerWorker();
    bool startServer(quint16 port, bool systemdSocket) override;
    bool isListenActive() override;
    TcpPeerWorkerInterfacePtr createServerPeerWorker(TcpPeer *peer, qintptr socketDescriptor) override;
private slots:
    void clientDisconnectedSRV(VeinTcp::TcpPeer *peer);
private:
    void incomingConnection(qintptr socketDescriptor) override;
    QList<TcpPeer*> m_clients;
    TcpServer *m_server = nullptr;
};
}
#endif // VEIN_TCP_SERVERPRIVATE_H
