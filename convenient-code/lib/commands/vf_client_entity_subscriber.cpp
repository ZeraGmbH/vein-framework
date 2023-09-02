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

void VfClientEntitySubscriber::sendSubscription()
{
    EntityData *eData = new EntityData();
    eData->setCommand(EntityData::Command::ECMD_SUBSCRIBE);
    eData->setEntityId(getEntityId());
    eData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(EntityData::EventTarget::ET_ALL);
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, eData);
    sendEvent(cEvent);
}

QStringList VfClientEntitySubscriber::getComponentNames()
{
    return m_componentNames;
}

void VfClientEntitySubscriber::parseIntrospectionData(EventData *evData)
{
    IntrospectionData *iData = static_cast<IntrospectionData *>(evData);
    QJsonObject jsonObject = iData->jsonData();
    QJsonArray componentsArray = jsonObject["components"].toArray();
    m_componentNames.clear();
    for(auto entry : componentsArray)
        m_componentNames.append(entry.toString());
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
