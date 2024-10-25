#ifndef VEIN_TCP_SERVER_H
#define VEIN_TCP_SERVER_H

#include "abstracttcpworkerfactory.h"
#include "vftcp_export.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkerinterface.h"
#include <QObject>

namespace VeinTcp
{
class VFTCP_EXPORT TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(AbstractTcpWorkerFactory *workerFactoryUsedShort, QObject *parent = nullptr);
    explicit TcpServer(QObject *parent = nullptr);
    bool isListening() const;
public slots:
    bool startServer(quint16 port, bool systemdSocket = true);
signals:
    void sigClientConnected(VeinTcp::TcpPeer *serverPeer);

private:
    TcpServerWorkerInterfacePtr m_serverWorker;
};
}
#endif // VEIN_TCP_SERVER_H
