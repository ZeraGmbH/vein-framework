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
public:
    TcpServerWorker(TcpServer *server);
    ~TcpServerWorker() override;
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
