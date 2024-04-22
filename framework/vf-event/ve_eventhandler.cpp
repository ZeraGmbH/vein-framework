#include "ve_eventhandler.h"
#include "ve_eventsystem.h"
#include <timerfactoryqt.h>
#include <QCoreApplication>
#include <QEvent>
#include <QList>
#include <QDataStream>

namespace VeinEvent
{
EventHandler::EventHandler(QObject *parent) :
    QObject(parent),
    m_waitForAddSilenceToLogTimer(TimerFactoryQt::createSingleShot(1000))
{
    connect(m_waitForAddSilenceToLogTimer.get(), &TimerTemplateQt::sigExpired,
            this, &EventHandler::onAddSilence);
}

EventHandler::~EventHandler()
{
    m_subsystems.clear();
}

QList<EventSystem *> EventHandler::subsystems() const
{
    return m_subsystems;
}

void EventHandler::setSubsystems(QList<EventSystem *> subsystems)
{
    if(m_subsystems != subsystems) {
        m_waitForAddSilenceToLogTimer->start();
        m_subsystems = subsystems;
        for(EventSystem *tmpSystem : qAsConst(m_subsystems))
            tmpSystem->attach(this);
        emit subsystemsChanged(m_subsystems);
    }
}

void EventHandler::addSubsystem(EventSystem *subsystem)
{
    Q_ASSERT(m_subsystems.contains(subsystem) == false);
    m_waitForAddSilenceToLogTimer->start();
    m_subsystems.append(subsystem);
    subsystem->attach(this);
    emit subsystemsChanged(m_subsystems);
}

void EventHandler::prependSubsystem(EventSystem *subsystem)
{
    Q_ASSERT(m_subsystems.contains(subsystem) == false);
    m_waitForAddSilenceToLogTimer->start();
    m_subsystems.prepend(subsystem);
    subsystem->attach(this);
    emit subsystemsChanged(m_subsystems);
}

void EventHandler::removeSubsystem(EventSystem *subsystem)
{
    subsystem->detach();
    m_subsystems.removeAll(subsystem);
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
    for(int i=0; i < m_subsystems.count(); ++i) {
        EventSystem* eventSystem = m_subsystems.at(i);
        eventSystem->processEvent(event);
        if(event->isAccepted()) {
            emit sigEventAccepted(eventSystem, event);
            break;
        }
    }
}

void EventHandler::onAddSilence()
{
    qInfo("Core event systems: %i", m_subsystems.count());
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
