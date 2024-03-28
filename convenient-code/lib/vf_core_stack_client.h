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
    void connectToServer(const QString &host, quint16 port);
    VeinNet::TcpSystem* getTcpSystem();
    void subscribeEntity(int entityId); // this should go once we got tasks

    VfCmdEventHandlerSystemPtr m_cmdEventHandlerSystem;
signals:
    void sigSubscribed(bool ok, int entityId);

private slots:
    void onSubscribed(bool ok);
private:
    VeinEvent::EventHandler m_eventHandler;
    VeinNet::NetworkSystem m_netSystem;
    VeinNet::TcpSystem m_tcpSystem;
    // first proof of concept for throw away after using once
    QHash<void*, VfCmdEventItemPtr> m_pendingCommandEventItems;
};

#endif // VFCORESTACKCLIENT_H
