#ifndef VEIN_TCP_SERVERPRIVATE_H
#define VEIN_TCP_SERVERPRIVATE_H

#include "vtcp_peer.h"
#include <QList>
#include <QTcpServer>

namespace VeinTcp
{
class TcpServer;

class TcpServerWorker : public QTcpServer
{
    Q_OBJECT
public:
    TcpServerWorker(TcpServer *server);
    ~TcpServerWorker();
    bool startServer(quint16 port, bool systemdSocket);
private slots:
    void clientDisconnectedSRV(TcpPeer *peer);
private:
    void incomingConnection(qintptr socketDescriptor) override;
    QList<TcpPeer*> m_clients;
    TcpServer *m_server = nullptr;
};
}
#endif // VEIN_TCP_SERVERPRIVATE_H
