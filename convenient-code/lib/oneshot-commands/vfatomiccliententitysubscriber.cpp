#include "vfatomiccliententitysubscriber.h"
#include "vcmp_entitydata.h"
#include <vcmp_introspectiondata.h>
#include <QJsonObject>
#include <QJsonArray>

using namespace VeinEvent;
using namespace VeinComponent;

std::shared_ptr<VfAtomicClientEntitySubscriber> VfAtomicClientEntitySubscriber::create(int entityId)
{
    return std::make_shared<VfAtomicClientEntitySubscriber>(entityId);
}

VfAtomicClientEntitySubscriber::VfAtomicClientEntitySubscriber(int entityId) :
    VfCmdEventItem(entityId)
{
}

void VfAtomicClientEntitySubscriber::sendSubscription()
{
    EntityData *eData = new EntityData();
    eData->setCommand(EntityData::Command::ECMD_SUBSCRIBE);
    eData->setEntityId(getEntityId());
    eData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(EntityData::EventTarget::ET_ALL);
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, eData);
    emit getEvenSystem()->sigSendEvent(cEvent);
}

QStringList VfAtomicClientEntitySubscriber::getComponentNames()
{
    return m_componentNames;
}

void VfAtomicClientEntitySubscriber::parseIntrospectionData(EventData *evData)
{
    IntrospectionData *iData = static_cast<IntrospectionData *>(evData);
    QJsonObject jsonObject = iData->jsonData();
    QJsonArray componentsArray = jsonObject["components"].toArray();
    m_componentNames.clear();
    for(auto entry : componentsArray)
        m_componentNames.append(entry.toString());
}

void VfAtomicClientEntitySubscriber::finishSubscription(bool ok)
{
    emit sigSubscribed(ok, getEntityId());
}

void VfAtomicClientEntitySubscriber::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    // we send entity data and receive introspection data
    if(evData->type() == IntrospectionData::dataType()) {
        parseIntrospectionData(evData);
        finishSubscription(evData->isValid());
    }
}

void VfAtomicClientEntitySubscriber::processErrorCommandEventData(VeinEvent::EventData *originalEventData)
{
    if(originalEventData->type() == EntityData::dataType()) {
        // again: we sent entity data
        EntityData *entityData = static_cast<EntityData *>(originalEventData);
        if(entityData->eventCommand() == EntityData::Command::ECMD_SUBSCRIBE)
            finishSubscription(false);
    }
}
