#include "vfcommandeventhandlersystem.h"

VfCommandEventHandlerSystem::VfCommandEventHandlerSystem(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter) :
    m_commandEventHandler(eventSubtypeFilter)
{
}

void VfCommandEventHandlerSystem::addItem(VfCommandEventHandlerItemPtr item)
{
    item->setEventSystem(this);
    m_commandEventHandler.addItem(item);
}

void VfCommandEventHandlerSystem::removeItem(VfCommandEventHandlerItemPtr item)
{
    m_commandEventHandler.removeItem(item);
}

void VfCommandEventHandlerSystem::processEvent(QEvent *event)
{
    m_commandEventHandler.processEvent(event);
}
