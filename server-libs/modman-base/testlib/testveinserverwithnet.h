#ifndef VFTESTSERVERSTACK_H
#define VFTESTSERVERSTACK_H

#include "testveinserver.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

class TestVeinServerWithNet
{
public:
    TestVeinServerWithNet(int serverPort, bool systemdScok = false);
    VeinEvent::EventHandler eventHandler;
private:
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    TestVeinServer server;
};

#endif // VFTESTSERVERSTACK_H
