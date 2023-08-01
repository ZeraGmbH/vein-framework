#include "vf_cmd_event_handler.h"
#include "vcmp_componentdata.h"
#include "vcmp_entitydata.h"
#include "vcmp_errordata.h"
#include "vcmp_introspectiondata.h"
#include "vcmp_remoteproceduredata.h"
#include "ve_eventdata.h"

using namespace VeinEvent;
using namespace VeinComponent;

VfCmdEventHandler::VfCmdEventHandler(CommandEvent::EventSubtype eventSubtypeFilter) :
    m_eventSubtypeFilter(eventSubtypeFilter)
{
}

void VfCmdEventHandler::addItem(VfCmdEventItemPtr item)
{
    CommandEventHandlerContainerType &items = m_items[item->getEntityId()];
    items.addItem(item);
}

void VfCmdEventHandler::removeItem(VfCmdEventItemPtr item)
{
    CommandEventHandlerContainerType &items = m_items[item->getEntityId()];
    items.removeItem(item);
}

void VfCmdEventHandler::processEvent(QEvent *event)
{
    if(event->type() == CommandEvent::eventType())
        processCommandEvent(static_cast<CommandEvent *>(event));
}

void VfCmdEventHandler::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    if(cmdEvent->eventSubtype() == m_eventSubtypeFilter) {
        VeinEvent::EventData *eventData = cmdEvent->eventData();
        int entityId = eventData->entityId();
        auto iter = m_items.find(entityId);
        if(iter != m_items.end()) {
            int eventDataType = eventData->type();
            if(eventDataType != ErrorData::dataType()) {
                for(auto item = iter.value().getFirst(); item!=0; item=iter.value().getNext())
                    item->processCommandEvent(cmdEvent);
            }
            else
                handleErrorData(eventData, iter);
        }
    }
}

void VfCmdEventHandler::handleErrorData(VeinEvent::EventData *&eventData,
                                            QHash<int, CommandEventHandlerContainerType>::iterator iter)
{
    VeinComponent::ErrorData *errData = static_cast<VeinComponent::ErrorData *>(eventData);
    int originalDataType = errData->originalDataType();
    switch (originalDataType) {
    case VCMP_COMPONENTDATA_DATATYPE: {
        ComponentData data;
        data.deserialize(errData->originalData());
        for (auto item = iter.value().getFirst(); item != 0;
             item = iter.value().getNext())
            item->processErrorCommandEventData(&data);
        break;
    }
    case VCMP_ENTITYDATA_DATATYPE: {
        EntityData data;
        data.deserialize(errData->originalData());
        for (auto item = iter.value().getFirst(); item != 0;
             item = iter.value().getNext())
            item->processErrorCommandEventData(&data);
        break;
    }
    case VCMP_INTROSPECTIONDATA_DATATYPE: {
        IntrospectionData data;
        data.deserialize(errData->originalData());
        for (auto item = iter.value().getFirst(); item != 0;
             item = iter.value().getNext())
            item->processErrorCommandEventData(&data);
        break;
    }
    case VCMP_ERRORDATA_DATATYPE: // cannot be nested see ErrorData::setOriginalData
        break;
    case VCMP_REMOTEPROCEDUREDATA_DATATYPE: {
        RemoteProcedureData data;
        data.deserialize(errData->originalData());
        for (auto item = iter.value().getFirst(); item != 0;
             item = iter.value().getNext())
            item->processErrorCommandEventData(&data);
        break;
    }
    }
}

