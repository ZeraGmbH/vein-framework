#include "vftestserverstack.h"
#include <QCoreApplication>
#include <QAbstractEventDispatcher>

VfTestServerStack::VfTestServerStack(int serverPort) :
    server(&eventHandler)
{
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    tcpSystem.startServer(serverPort, false);
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
