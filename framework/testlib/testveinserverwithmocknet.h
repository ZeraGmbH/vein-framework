#ifndef TESTVEINSERVERWITHMOCKNET_H
#define TESTVEINSERVERWITHMOCKNET_H

#include "testveinserver.h"
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"

class TestVeinServerWithMockNet
{
public:
    TestVeinServerWithMockNet(int serverPort, QList<VeinEvent::EventSystem*> aboveNetworkSystems = QList<VeinEvent::EventSystem*>());
    TestVeinServer* getServer();
    VeinStorage::AbstractEventSystem* getStorage();
    int getSubscriberCount(int entityId);
private:
    VeinNet::NetworkSystem m_netSystem;
    VeinNet::TcpSystem m_tcpSystem;
    TestVeinServer m_server;
};

#endif // TESTVEINSERVERWITHMOCKNET_H
