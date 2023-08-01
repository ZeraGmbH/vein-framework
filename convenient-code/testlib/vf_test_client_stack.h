#ifndef VFTESTCLIENTSTACK_H
#define VFTESTCLIENTSTACK_H

#include "ve_eventhandler.h"
#include "vf_cmd_event_handler_system.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

struct VfTestClientStack
{
    VeinEvent::EventHandler eventHandler;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    VfTestClientStack();
    void subscribeEntityId(int entityId, VfCmdEventHandlerSystem *cmdEventHandlerSystem);
};

#endif // VFTESTCLIENTSTACK_H
