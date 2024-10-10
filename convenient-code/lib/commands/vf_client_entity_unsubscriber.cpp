#include "vf_client_entity_unsubscriber.h"
#include "vcmp_entitydata.h"

std::shared_ptr<VfClientEntityUnsubscriber> VfClientEntityUnsubscriber::create(int entityId)
{
    return std::make_shared<VfClientEntityUnsubscriber>(entityId);
}

VfClientEntityUnsubscriber::VfClientEntityUnsubscriber(int entityId) :
    VfEntityEventItem(entityId)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

QEvent *VfClientEntityUnsubscriber::generateEvent(int entityId)
{
    EntityData *eData = new EntityData();
    eData->setCommand(EntityData::Command::ECMD_UNSUBSCRIBE);
    eData->setEntityId(entityId);
    eData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(EntityData::EventTarget::ET_ALL);
    return new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, eData);
}

void VfClientEntityUnsubscriber::sendUnsubscription()
{
    sendEvent(generateEvent(getEntityId()));
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

