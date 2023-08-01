#include "vf_cmd_event_handler_system.h"

std::shared_ptr<VfCmdEventHandlerSystem> VfCmdEventHandlerSystem::create(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter)
{
    return std::make_shared<VfCmdEventHandlerSystem>(eventSubtypeFilter);
}

VfCmdEventHandlerSystem::VfCmdEventHandlerSystem(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter) :
    m_commandEventHandler(eventSubtypeFilter)
{
}

void VfCmdEventHandlerSystem::addItem(VfCmdEventItemPtr item)
{
    item->setEventSystem(this);
    m_commandEventHandler.addItem(item);
}

void VfCmdEventHandlerSystem::removeItem(VfCmdEventItemPtr item)
{
    m_commandEventHandler.removeItem(item);
}

void VfCmdEventHandlerSystem::processEvent(QEvent *event)
{
    m_commandEventHandler.processEvent(event);
}
