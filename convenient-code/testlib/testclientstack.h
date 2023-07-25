#ifndef TESTCLIENTSTACK_H
#define TESTCLIENTSTACK_H

#include "ve_eventhandler.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

struct TestClientStack
{
    VeinEvent::EventHandler eventHandler;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    TestClientStack();
};

#endif // TESTCLIENTSTACK_H
