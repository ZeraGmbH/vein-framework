#include "ve_eventhandler.h"

#include "ve_eventsystem.h"
#include <QEvent>
#include "vh_logging.h"

#include <QCoreApplication>
#include <QList>
#include <QDataStream>

namespace VeinEvent
{
EventHandler::EventHandler(QObject *parent) :
    QObject(parent)
{
    vCDebug(VEIN_EVENT) << "Created event handler";
}

QList<EventSystem *> EventHandler::subsystems() const
{
    return m_subsystems;
}

void EventHandler::setSubsystems(QList<EventSystem *> subsystems)
{
    if(m_subsystems != subsystems) {
        m_subsystems = subsystems;
        for(EventSystem *tmpSystem : qAsConst(m_subsystems))
            tmpSystem->attach(this);
        emit subsystemsChanged(m_subsystems);
    }
}

void EventHandler::addSubsystem(EventSystem *subsystem)
{
    Q_ASSERT(m_subsystems.contains(subsystem) == false);
    m_subsystems.append(subsystem);
    subsystem->attach(this);
    emit subsystemsChanged(m_subsystems);
}

void EventHandler::clearSubsystems()
{
    if(m_subsystems.isEmpty() == false) {
        m_subsystems.clear();
        emit subsystemsChanged(m_subsystems);
    }
}

void EventHandler::customEvent(QEvent *event)
{
    Q_ASSERT(event != nullptr);
    /**
     * @note some speed could be gained by using a subscription based n:m (entity:system) mapping so that systems only get notified about the entities they care about
     * @todo maybe event processing can be accelerated with QtConcurrent / OpenMP?
     */

    for(int i=0; i < m_subsystems.count() && event->isAccepted()==false; ++i)
        m_subsystems.at(i)->processEvent(event);
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
