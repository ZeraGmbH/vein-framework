#include "vf_test_server_stack.h"

VfTestServerStack::VfTestServerStack(int serverPort, bool systemdScok) :
    server(&eventHandler)
{
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    tcpSystem.startServer(serverPort, systemdScok);
}
