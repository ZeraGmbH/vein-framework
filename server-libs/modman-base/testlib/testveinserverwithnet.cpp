#include "testveinserverwithnet.h"

TestVeinServerWithNet::TestVeinServerWithNet(int serverPort, QList<VeinEvent::EventSystem *> aboveNetworkSystems) :
    m_server(&m_eventHandler)
{
    for(const auto system : qAsConst(aboveNetworkSystems))
        m_eventHandler.addSubsystem(system);
    m_eventHandler.addSubsystem(&m_netSystem);
    m_eventHandler.addSubsystem(&m_tcpSystem);
    m_tcpSystem.startServer(serverPort, false);
}

VeinEvent::EventHandler *TestVeinServerWithNet::getEventHandler()
{
    return &m_eventHandler;
}
