#include "testveinserverwithnet.h"
#include <timemachineobject.h>

TestVeinServerWithNet::TestVeinServerWithNet(int serverPort, QList<VeinEvent::EventSystem *> aboveNetworkSystems)
{
    for(const auto system : qAsConst(aboveNetworkSystems))
        m_server.appendEventSystem(system);
    m_server.appendEventSystem(&m_netSystem);
    m_server.appendEventSystem(&m_tcpSystem);
    m_tcpSystem.startServer(serverPort, false);
    TimeMachineObject::feedEventLoop();
}

TestVeinServer *TestVeinServerWithNet::getServer()
{
    return &m_server;
}

VeinEvent::StorageSystem *TestVeinServerWithNet::getStorage()
{
    return getServer()->getStorage();
}

int TestVeinServerWithNet::getSubscriberCount(int entityId)
{
    return m_netSystem.getSubscriberCount(entityId);
}
