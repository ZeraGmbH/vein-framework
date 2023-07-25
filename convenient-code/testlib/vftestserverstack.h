#ifndef VFTESTSERVERSTACK_H
#define VFTESTSERVERSTACK_H

#include "veintestserver.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

struct VfTestServerStack
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    VfTestServerStack(int serverPort);
};

#endif // VFTESTSERVERSTACK_H
