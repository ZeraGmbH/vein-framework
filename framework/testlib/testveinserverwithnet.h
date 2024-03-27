#ifndef VFTESTSERVERSTACK_H
#define VFTESTSERVERSTACK_H

#include "testveinserver.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

class TestVeinServerWithNet
{
public:
    TestVeinServerWithNet(int serverPort, QList<VeinEvent::EventSystem*> aboveNetworkSystems = QList<VeinEvent::EventSystem*>());
    VeinEvent::EventHandler* getEventHandler();
    TestVeinServer* getServer();
    VeinEvent::StorageSystem* getStorage();
private:
    VeinEvent::EventHandler m_eventHandler;
    VeinNet::NetworkSystem m_netSystem;
    VeinNet::TcpSystem m_tcpSystem;
    TestVeinServer m_server;
};

#endif // VFTESTSERVERSTACK_H
