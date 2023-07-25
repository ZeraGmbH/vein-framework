#include "testserverstack.h"
#include <QCoreApplication>
#include <QAbstractEventDispatcher>

TestServerStack::TestServerStack(int serverPort) :
    server(&eventHandler)
{
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    tcpSystem.startServer(serverPort, false);
    while(QCoreApplication::eventDispatcher()->processEvents(QEventLoop::AllEvents));
}
