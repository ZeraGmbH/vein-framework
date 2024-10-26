#include "testveinserverwithmocknet.h"
#include "mocktcpworkerfactory.h"
#include <timemachineobject.h>

TestVeinServerWithMockNet::TestVeinServerWithMockNet(int serverPort, QList<VeinEvent::EventSystem *> aboveNetworkSystems) :
    m_tcpSystem(VeinTcp::MockTcpWorkerFactory::create())
{
    for(const auto system : qAsConst(aboveNetworkSystems))
        m_server.appendEventSystem(system);
    m_server.appendEventSystem(&m_netSystem);
    m_server.appendEventSystem(&m_tcpSystem);
    m_tcpSystem.startServer(serverPort, false);
    TimeMachineObject::feedEventLoop();
}

TestVeinServer *TestVeinServerWithMockNet::getServer()
{
    return &m_server;
}

VeinEvent::StorageSystem *TestVeinServerWithMockNet::getStorage()
{
    return getServer()->getStorage();
}

int TestVeinServerWithMockNet::getSubscriberCount(int entityId)
{
    return m_netSystem.getSubscriberCount(entityId);
}
