#ifndef VFCORESTACKCLIENT_H
#define VFCORESTACKCLIENT_H

#include "ve_eventhandler.h"
#include "vf_cmd_event_handler_system.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"
#include <QHash>

// * Minimum facade for a vein client
// * Application should APPEND further event systems to eventHandler
class VfCoreStackClient : public QObject
{
    Q_OBJECT
public:
    VfCoreStackClient();
    VfCoreStackClient(VeinTcp::AbstractTcpNetworkFactoryPtr tcpNetworkFactory);
    void connectToServer(const QString &host, quint16 port);
    void subscribeEntity(int entityId); // this should go once we got tasks
    void unsubscribeEntity(int entityId);

    void appendEventSystem(VeinEvent::EventSystem* system);
    void prependEventSystem(VeinEvent::EventSystem* system);
    void addItem(VfCmdEventItemPtr item);
    VeinEvent::EventHandler *getEventHandler();
    VfCmdEventHandlerSystemPtr getCmdEventHandlerSystem();
signals:
    void sigConnnectionEstablished();
    void sigSubscribed(bool ok, int entityId);
    void sigUnsubscribed(bool ok, int entityId);

private slots:
    void onSubscribed(bool ok);
    void onUnsubscribed(bool ok);
private:
    void init();
    VeinEvent::EventHandler m_eventHandler;
    VfCmdEventHandlerSystemPtr m_cmdEventHandlerSystem;
    VeinNet::NetworkSystem m_netSystem;
    VeinNet::TcpSystem m_tcpSystem;
    QHash<void*, VfCmdEventItemPtr> m_pendingCommandEventItems;
};

#endif // VFCORESTACKCLIENT_H
