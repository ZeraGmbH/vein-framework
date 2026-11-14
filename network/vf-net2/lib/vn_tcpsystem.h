#ifndef VN_TCPSYSTEM_H
#define VN_TCPSYSTEM_H

#include "globalIncludes.h"
#include "abstracttcpnetworkfactory.h"
#include <ve_eventsystem.h>
#include <QAbstractSocket>
#include <QUuid>

namespace VeinTcp
{
class TcpPeer;
class TcpServer;
}

namespace VeinNet
{
class VFNET2_EXPORT TcpSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit TcpSystem(const VeinTcp::AbstractTcpNetworkFactoryPtr &tcpNetworkFactory, QObject *parent = nullptr);
    virtual ~TcpSystem();

    void processEvent(QEvent *t_event) override;
signals:
    // client part
    void sigConnnectionEstablished(const QUuid &t_connectionId);
    // server part
    void sigClientConnected(const QUuid &t_connectionId);
    void sigClientDisconnected(const QUuid &t_connectionId);
public slots:
    bool startServer(quint16 t_port, bool t_systemdSocket=true);
    void connectToServer(const QString &t_host, quint16 t_port);

private slots:
    void onClientConnected(VeinTcp::TcpPeer *t_networkPeer);
    void onConnectionEstablished(VeinTcp::TcpPeer *t_peer);
    void onConnectionClosed(VeinTcp::TcpPeer *t_peer);
    void onClientDisconnected(VeinTcp::TcpPeer *t_peer);
    void onMessageReceived(VeinTcp::TcpPeer *sender, const QByteArray &buffer);
    void onSocketError(VeinTcp::TcpPeer *t_peer, QAbstractSocket::SocketError t_socketError);
private:
    void init();
    QList<VeinTcp::TcpPeer *> m_waitingAuth;
    QHash<QUuid, VeinTcp::TcpPeer *> m_peerList;
    VeinTcp::AbstractTcpNetworkFactoryPtr m_tcpNetworkFactory;
    VeinTcp::TcpServer *m_server = nullptr;
};
}
#endif // VN_TCPSYSTEM_H
