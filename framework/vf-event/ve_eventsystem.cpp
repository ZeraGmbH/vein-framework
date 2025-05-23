#include "ve_eventsystem.h"
#include "ve_eventhandler.h"
#include "vh_logging.h"
#include <QCoreApplication>

Q_LOGGING_CATEGORY(VEIN_EVENT, VEIN_DEBUGNAME_EVENT)
Q_LOGGING_CATEGORY(VEIN_SCRIPTEVENT, VEIN_DEBUGNAME_SCRIPT)

namespace VeinEvent
{
int EventSystem::m_instanceCount = 0;

EventSystem::EventSystem(QObject *parent) :
    QObject(parent)
{
    m_instanceCount++;
}

VeinEvent::EventSystem::~EventSystem()
{
    m_instanceCount--;
}

void EventSystem::attach(EventHandler *eventHandler)
{
    Q_ASSERT(eventHandler != nullptr);
    VF_ASSERT(!m_eventHandler, "EventSystem already attached");

    connect(this, &VeinEvent::EventSystem::sigSendEvent,
            this, &VeinEvent::EventSystem::onSendEvent);
    m_eventHandler = eventHandler;
    emit sigAttached();
}

void EventSystem::detach()
{
    disconnect(this, &VeinEvent::EventSystem::sigSendEvent,
               this, &VeinEvent::EventSystem::onSendEvent);
    m_eventHandler = nullptr;
}

int EventSystem::getInstanceCount()
{
    return m_instanceCount;
}

void EventSystem::onSendEvent(QEvent *event)
{
    QCoreApplication::instance()->postEvent(m_eventHandler, event);
}
}
