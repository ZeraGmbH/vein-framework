#include "vfcmdeventitementity.h"
#include "vfcmdeventitemcomponent.h"

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
    Q_ASSERT(evData != nullptr);
    if(evData->type() == ComponentData::dataType()) {
        ComponentData *cData = static_cast<ComponentData *>(evData);
        const QString &componentName = cData->componentName();
        auto iter = m_componentItems.find(componentName);
        if(iter != m_componentItems.end()) {
            for(auto item = iter.value().getFirst(); item!=0; item=iter.value().getNext())
                item->processComponentEventData(cData);
        }
    }
}

void VfCmdEventItemEntity::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    if(originalEventData->type() == ComponentData::dataType()) {
        ComponentData *cData = static_cast<ComponentData *>(originalEventData);
        const QString &componentName = cData->componentName();
        auto iter = m_componentItems.find(componentName);
        if(iter != m_componentItems.end()) {
            for(auto item = iter.value().getFirst(); item!=0; item=iter.value().getNext())
                item->processComponentEventData(cData);
        }
    }

}
