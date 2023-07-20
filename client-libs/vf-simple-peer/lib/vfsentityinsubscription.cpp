#include "vfsentityinsubscription.h"
#include "vcmp_entitydata.h"
#include "vcmp_errordata.h"
#include <vcmp_introspectiondata.h>
#include <QJsonObject>
#include <QJsonArray>

using namespace VeinEvent;
using namespace VeinComponent;

std::shared_ptr<VfsEntityInSubscription> VfsEntityInSubscription::create(int entityId)
{
    return std::make_shared<VfsEntityInSubscription>(entityId);
}

VfsEntityInSubscription::VfsEntityInSubscription(int entityId) :
    VfCommandEventHandlerItem(entityId)
{
}

void VfsEntityInSubscription::sendSubscrption()
{
    EntityData *eData = new EntityData();
    eData->setCommand(EntityData::Command::ECMD_SUBSCRIBE);
    eData->setEntityId(m_entityId);
    eData->setEventOrigin(EntityData::EventOrigin::EO_LOCAL);
    eData->setEventTarget(EntityData::EventTarget::ET_ALL);
    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::TRANSACTION, eData);
    emit m_eventSystem->sigSendEvent(cEvent);
}

QStringList VfsEntityInSubscription::getComponentNames()
{
    return m_componentNames;
}

void VfsEntityInSubscription::parseIntrospectionData(EventData *evData)
{
    IntrospectionData *iData = static_cast<IntrospectionData *>(evData);
    QJsonObject jsonObject = iData->jsonData();
    QJsonArray componentsArray = jsonObject["components"].toArray();
    m_componentNames.clear();
    for(auto entry : componentsArray)
        m_componentNames.append(entry.toString());
}

void VfsEntityInSubscription::processCommandEvent(VeinEvent::CommandEvent *cmdEvent)
{
    if(cmdEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION) {
        EventData *evData = cmdEvent->eventData();
        Q_ASSERT(evData != nullptr);
        switch(evData->type()) {
            case IntrospectionData::dataType():
                parseIntrospectionData(evData);
                emit sigSubscribed(true, m_entityId);
                break;
            case ErrorData::dataType():
                emit sigSubscribed(false, m_entityId);
                break;
        }
    }
}

