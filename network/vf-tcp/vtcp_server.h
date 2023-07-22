#ifndef VEIN_TCP_SERVER_H
#define VEIN_TCP_SERVER_H

#include "vftcp_export.h"
#include "vtcp_peer.h"
#include <QObject>

namespace VeinTcp
{
class TcpServerWorker;

class VFTCP_EXPORT TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    virtual ~TcpServer();
    bool isListening() const;
public slots:
    bool startServer(quint16 port, bool systemdSocket = true);
signals:
    void sigClientConnected(TcpPeer *t_peer);

private:
    friend class TcpServerWorker;
    TcpServerWorker *m_serverWorker = nullptr;
};
}
#endif // VEIN_TCP_SERVER_H
