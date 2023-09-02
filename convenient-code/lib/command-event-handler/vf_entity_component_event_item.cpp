#include "vf_entity_component_event_item.h"
#include "vf_component_event_item.h"

std::shared_ptr<VfEntityComponentEventItem> VfEntityComponentEventItem::create(int entityId)
{
    return std::make_shared<VfEntityComponentEventItem>(entityId);
}

VfEntityComponentEventItem::VfEntityComponentEventItem(int entityId) :
    VfEntityEventItem(entityId)
{
}

void VfEntityComponentEventItem::addItem(VfCmdEventItemComponentPtr item)
{
    m_componentItems[item->getComponentName()].addItem(item);
}

void VfEntityComponentEventItem::removeItem(VfCmdEventItemComponentPtr item)
{
    m_componentItems[item->getComponentName()].removeItem(item);
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfEntityComponentEventItem::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
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

void VfEntityComponentEventItem::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
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
