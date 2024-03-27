#include "moduleeventhandler.h"
#include <QEvent>

ModuleEventHandler::ModuleEventHandler(QObject *parent) : VeinEvent::EventHandler(parent)
{
}

void ModuleEventHandler::addModuleSystem(VeinEvent::EventSystem *eventSystem)
{
    if(m_moduleSystems.contains(eventSystem) == false) {
        m_moduleSystems.append(eventSystem);
        eventSystem->attach(this);
    }
}

void ModuleEventHandler::clearModuleSystems()
{
    m_moduleSystems.clear();
}

void ModuleEventHandler::customEvent(QEvent *event)
{
    for(int i=0; i < m_moduleSystems.count() && event->isAccepted()==false; ++i)
        m_moduleSystems.at(i)->processEvent(event);
    VeinEvent::EventHandler::customEvent(event);
}
