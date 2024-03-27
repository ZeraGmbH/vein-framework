#include "testveinserverwithnet.h"

TestVeinServerWithNet::TestVeinServerWithNet(int serverPort, bool systemdScok) :
    server(&eventHandler)
{
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    tcpSystem.startServer(serverPort, systemdScok);
}
