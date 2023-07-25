#ifndef TESTSERVERSTACK_H
#define TESTSERVERSTACK_H

#include "veintestserver.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

struct TestServerStack
{
    VeinEvent::EventHandler eventHandler;
    VeinTestServer server;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    TestServerStack(int serverPort);
};

#endif // TESTSERVERSTACK_H
