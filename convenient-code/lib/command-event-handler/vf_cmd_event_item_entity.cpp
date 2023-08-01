#include "vf_cmd_event_item_entity.h"
#include "vf_cmd_event_item_component.h"

std::shared_ptr<VfCmdEventItemEntity> VfCmdEventItemEntity::create(int entityId)
{
    return std::make_shared<VfCmdEventItemEntity>(entityId);
}

VfCmdEventItemEntity::VfCmdEventItemEntity(int entityId) :
    VfCmdEventItem(entityId)
{
}

void VfCmdEventItemEntity::addItem(VfCmdEventItemComponentPtr item)
{
    m_componentItems[item->getComponentName()].addItem(item);
}

void VfCmdEventItemEntity::removeItem(VfCmdEventItemComponentPtr item)
{
    m_componentItems[item->getComponentName()].removeItem(item);
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfCmdEventItemEntity::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    if(evData->type() == ComponentData::dataType()) {
        ComponentData *componentData = static_cast<ComponentData *>(evData);
        auto iter = m_componentItems.find(componentData->componentName());
        if(iter != m_componentItems.end()) {
            for(VfCmdEventItemComponentPtr item = iter.value().getFirst(); item!=nullptr; item=iter.value().getNext())
                item->processComponentEventData(componentData);
        }
    }
}

void VfCmdEventItemEntity::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    if(originalEventData->type() == ComponentData::dataType()) {
        ComponentData *componentData = static_cast<ComponentData *>(originalEventData);
        auto iter = m_componentItems.find(componentData->componentName());
        if(iter != m_componentItems.end()) {
            for(VfCmdEventItemComponentPtr item = iter.value().getFirst(); item!=nullptr; item=iter.value().getNext())
                item->processComponentEventData(componentData);
        }
    }
}
