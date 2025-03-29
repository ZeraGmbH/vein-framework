#include "vf_client_entity_subscriber.h"
#include "vcmp_entitydata.h"
#include <vcmp_introspectiondata.h>
#include <QJsonObject>
#include <QJsonArray>

std::shared_ptr<VfClientEntitySubscriber> VfClientEntitySubscriber::create(int entityId)
{
    return std::make_shared<VfClientEntitySubscriber>(entityId);
}

VfClientEntitySubscriber::VfClientEntitySubscriber(int entityId) :
    VfEntityEventItem(entityId)
{
}

using namespace VeinEvent;
using namespace VeinComponent;

QEvent *VfClientEntitySubscriber::generateEvent(int entityId)
{
    EntityData *eData = new EntityData();
    eData->setCommand(EntityData::Command::ECMD_SUBSCRIBE);
    eData->setEntityId(entityId);
    eData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(EntityData::EventTarget::ET_ALL);
    return new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, eData);
}

void VfClientEntitySubscriber::sendSubscription()
{
    sendEvent(generateEvent(getEntityId()));
}

QStringList VfClientEntitySubscriber::getComponentNames() const
{
    return m_components.keys();
}

QVariantMap VfClientEntitySubscriber::getComponents() const
{
    return m_components;
}

void VfClientEntitySubscriber::parseIntrospectionData(EventData *evData)
{
    IntrospectionData *iData = static_cast<IntrospectionData *>(evData);
    m_components = iData->componentValues();
}

void VfClientEntitySubscriber::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    // we send entity data and receive introspection data
    if(evData->type() == IntrospectionData::dataType()) {
        parseIntrospectionData(evData);
        emitSigSubscribed(evData->isValid());
    }
}

void VfClientEntitySubscriber::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    if(originalEventData->type() == EntityData::dataType()) {
        // again: we sent entity data
        EntityData *entityData = static_cast<EntityData *>(originalEventData);
        if(entityData->eventCommand() == EntityData::Command::ECMD_SUBSCRIBE)
            emitSigSubscribed(false);
    }
}

void VfClientEntitySubscriber::emitSigSubscribed(bool ok)
{
    emit sigSubscribed(ok, getEntityId());
}
