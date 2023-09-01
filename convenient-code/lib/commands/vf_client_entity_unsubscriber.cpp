#include "vf_client_entity_unsubscriber.h"
#include "vcmp_entitydata.h"

std::shared_ptr<VfClientEntityUnsubscriber> VfClientEntityUnsubscriber::create(int entityId)
{
    return std::make_shared<VfClientEntityUnsubscriber>(entityId);
}

VfClientEntityUnsubscriber::VfClientEntityUnsubscriber(int entityId) :
    VfCmdEventItem(entityId)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

void VfClientEntityUnsubscriber::sendUnsubscription()
{
    EntityData *eData = new EntityData();
    eData->setCommand(EntityData::Command::ECMD_UNSUBSCRIBE);
    eData->setEntityId(getEntityId());
    eData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(EntityData::EventTarget::ET_ALL);
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, eData);
    emit getEventSystem()->sigSendEvent(cEvent);
}

void VfClientEntityUnsubscriber::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    if(cmdEvent->type() == EntityData::dataType()) {
        EntityData *entityData = static_cast<EntityData *>(cmdEvent->eventData());
        if(entityData->eventCommand() == EntityData::Command::ECMD_UNSUBSCRIBE)
            emitSigUnsubscribed(true);
    }
}

void VfClientEntityUnsubscriber::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    if(originalEventData->type() == EntityData::dataType()) {
        EntityData *entityData = static_cast<EntityData *>(originalEventData);
        if(entityData->eventCommand() == EntityData::Command::ECMD_UNSUBSCRIBE)
            emitSigUnsubscribed(false);
    }
}

void VfClientEntityUnsubscriber::emitSigUnsubscribed(bool ok)
{
    emit sigUnsubscribed(ok, getEntityId());
}

