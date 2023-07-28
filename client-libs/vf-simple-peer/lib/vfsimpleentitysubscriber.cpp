#include "vfsimpleentitysubscriber.h"
#include "vcmp_entitydata.h"
#include "vcmp_errordata.h"
#include <vcmp_introspectiondata.h>
#include <QJsonObject>
#include <QJsonArray>

using namespace VeinEvent;
using namespace VeinComponent;

std::shared_ptr<VfSimpleEntitySubscriber> VfSimpleEntitySubscriber::create(int entityId)
{
    return std::make_shared<VfSimpleEntitySubscriber>(entityId);
}

VfSimpleEntitySubscriber::VfSimpleEntitySubscriber(int entityId) :
    VfCmdEventItem(entityId)
{
}

void VfSimpleEntitySubscriber::sendSubscription()
{
    EntityData *eData = new EntityData();
    eData->setCommand(EntityData::Command::ECMD_SUBSCRIBE);
    eData->setEntityId(getEntityId());
    eData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(EntityData::EventTarget::ET_ALL);
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, eData);
    emit m_eventSystem->sigSendEvent(cEvent);
}

QStringList VfSimpleEntitySubscriber::getComponentNames()
{
    return m_componentNames;
}

void VfSimpleEntitySubscriber::parseIntrospectionData(EventData *evData)
{
    IntrospectionData *iData = static_cast<IntrospectionData *>(evData);
    QJsonObject jsonObject = iData->jsonData();
    QJsonArray componentsArray = jsonObject["components"].toArray();
    m_componentNames.clear();
    for(auto entry : componentsArray)
        m_componentNames.append(entry.toString());
}

void VfSimpleEntitySubscriber::finishSubscription(bool ok)
{
    emit sigSubscribed(ok, getEntityId());
}

void VfSimpleEntitySubscriber::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    EventData *evData = cmdEvent->eventData();
    Q_ASSERT(evData != nullptr);
    switch(evData->type()) {
    case IntrospectionData::dataType():
        parseIntrospectionData(evData);
        finishSubscription(true);
        break;
    case ErrorData::dataType():
        finishSubscription(false);
        break;
    }
}

