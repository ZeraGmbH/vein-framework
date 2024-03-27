#ifndef VFTESTSERVERSTACK_H
#define VFTESTSERVERSTACK_H

#include "testveinserver.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

class TestVeinServerWithNet
{
public:
    TestVeinServerWithNet(int serverPort, bool systemdSocket = false);
    VeinEvent::EventHandler* getEventHandler();
private:
    VeinEvent::EventHandler m_eventHandler;
    VeinNet::NetworkSystem m_netSystem;
    VeinNet::TcpSystem m_tcpSystem;
    TestVeinServer m_server;
};

#endif // VFTESTSERVERSTACK_H
