#include "testveinserverwithnet.h"

TestVeinServerWithNet::TestVeinServerWithNet(int serverPort, bool systemdScok) :
    m_server(&m_eventHandler)
{
    m_eventHandler.addSubsystem(&m_netSystem);
    m_eventHandler.addSubsystem(&m_tcpSystem);
    m_tcpSystem.startServer(serverPort, systemdScok);
}

VeinEvent::EventHandler *TestVeinServerWithNet::getEventHandler()
{
    return &m_eventHandler;
}
