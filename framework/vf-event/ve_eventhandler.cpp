#include "ve_eventhandler.h"

#include "ve_eventsystem.h"
#include <QEvent>
#include "vh_logging.h"

#include <QCoreApplication>
#include <QList>
#include <QDataStream>

namespace VeinEvent
{
EventHandler::EventHandler(QObject *t_parent) :
    QObject(t_parent)
{
    vCDebug(VEIN_EVENT) << "Created event handler";
}

QList<EventSystem *> EventHandler::subsystems() const
{
    return m_subsystems;
}

void EventHandler::setSubsystems(QList<EventSystem *> t_subsystems)
{
    if(m_subsystems != t_subsystems) {
        m_subsystems = t_subsystems;
        for(EventSystem *tmpSystem : qAsConst(m_subsystems))
            tmpSystem->attach(this);
        emit subsystemsChanged(m_subsystems);
    }
}

void EventHandler::addSubsystem(EventSystem *t_subsystem)
{
    Q_ASSERT(m_subsystems.contains(t_subsystem) == false);
    m_subsystems.append(t_subsystem);
    t_subsystem->attach(this);
    emit subsystemsChanged(m_subsystems);
}

void EventHandler::clearSubsystems()
{
    if(m_subsystems.isEmpty() == false) {
        m_subsystems.clear();
        emit subsystemsChanged(m_subsystems);
    }
}

void EventHandler::customEvent(QEvent *t_event)
{
    Q_ASSERT(t_event != nullptr);
    /**
     * @note some speed could be gained by using a subscription based n:m (entity:system) mapping so that systems only get notified about the entities they care about
     * @todo maybe event processing can be accelerated with QtConcurrent / OpenMP?
     */

    for(int i=0; i < m_subsystems.count() && t_event->isAccepted()==false; ++i)
        m_subsystems.at(i)->processEvent(t_event);
}

void registerStreamOperators()
{
    qRegisterMetaTypeStreamOperators<QList<double> >("QList<double>");
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
    qRegisterMetaTypeStreamOperators<QList<QString> >("QList<QString>");
    qRegisterMetaTypeStreamOperators<QVariantMap>("QVariantMap");
    qRegisterMetaTypeStreamOperators<QVariantList>("QVariantList");
}

Q_COREAPP_STARTUP_FUNCTION(registerStreamOperators)
}
