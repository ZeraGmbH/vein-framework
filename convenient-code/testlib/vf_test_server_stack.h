#ifndef VFTESTSERVERSTACK_H
#define VFTESTSERVERSTACK_H

#include "veintestserver.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

class VfTestServerStack
{
public:
    VfTestServerStack(int serverPort, bool systemdScok = false);
    VeinEvent::EventHandler eventHandler;
private:
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    VeinTestServer server;
};

#endif // VFTESTSERVERSTACK_H
