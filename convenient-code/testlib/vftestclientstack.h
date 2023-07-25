#ifndef VFTESTCLIENTSTACK_H
#define VFTESTCLIENTSTACK_H

#include "ve_eventhandler.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

struct VfTestClientStack
{
    VeinEvent::EventHandler eventHandler;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    VfTestClientStack();
};

#endif // VFTESTCLIENTSTACK_H
