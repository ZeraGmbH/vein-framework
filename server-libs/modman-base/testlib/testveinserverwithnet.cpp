#include "testveinserverwithnet.h"

TestVeinServerWithNet::TestVeinServerWithNet(int serverPort, bool systemdSocket) :
    m_server(&m_eventHandler)
{
    m_eventHandler.addSubsystem(&m_netSystem);
    m_eventHandler.addSubsystem(&m_tcpSystem);
    m_tcpSystem.startServer(serverPort, systemdSocket);
}

VeinEvent::EventHandler *TestVeinServerWithNet::getEventHandler()
{
    return &m_eventHandler;
}
