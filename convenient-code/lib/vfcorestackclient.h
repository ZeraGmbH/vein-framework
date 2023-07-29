#ifndef VFCORESTACKCLIENT_H
#define VFCORESTACKCLIENT_H

#include "ve_eventhandler.h"
#include "vfcommandeventhandlersystem.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"
#include <QHash>

// * Minimum facade for a vein client
// * Application should append further event systems to eventHandler
// * Example usages are found in tests here and vf-debugger
class VfCoreStackClient : public QObject
{
    Q_OBJECT
public:
    VfCoreStackClient();
    void subscribeEntity(int entityId); // yes clients use vfqml but...
    VeinEvent::EventHandler eventHandler;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    VfCommandEventHandlerSystem cmdEventHandlerSystem;
signals:
    void sigSubscribed(bool ok, int entityId);

private slots:
    void onSubscribed(bool ok);
private:
    // first proof of concept for throw away after using once
    QHash<void*, VfCmdEventItemPtr> m_pendingCommandEventItems;
};

#endif // VFCORESTACKCLIENT_H
