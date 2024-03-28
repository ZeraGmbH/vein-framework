#include "testcommandeventspyeventsystem.h"
#include <vcmp_componentdata.h>
#include <vcmp_errordata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_remoteproceduredata.h>
#include <QJsonArray>
#include <QJsonDocument>

using namespace VeinEvent;
using namespace VeinComponent;

void TestCommandEventSpyEventSystem::processEvent(QEvent *event)
{
    if(event->type() == CommandEvent::getQEventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        Q_ASSERT(cEvent != nullptr);
        EventData *evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);
        QJsonObject jsonEventInfo = commonInfo(cEvent, evData);

        switch(evData->type()) {
        case EntityData::dataType():
            handleEntityData(evData, jsonEventInfo);
            break;
        case ComponentData::dataType():
            handleComponentData(evData, jsonEventInfo);
            break;
        case IntrospectionData::dataType():
            handleIntrospectionData(evData, jsonEventInfo);
            break;
        case RemoteProcedureData::dataType():
            handleRpcData(evData, jsonEventInfo);
            break;
        case ErrorData::dataType():
            handleErrorData(evData, jsonEventInfo);
            break;
        default:
            qFatal("Unknown event data!");
        }
        addJsonInfo(jsonEventInfo);
    }
}

void TestCommandEventSpyEventSystem::clear()
{
    m_jsonEvents = QJsonObject();
}

bool TestCommandEventSpyEventSystem::isEmpty() const
{
    return m_jsonEvents.isEmpty();
}

QByteArray TestCommandEventSpyEventSystem::dumpToJson()
{
    QJsonDocument doc(m_jsonEvents);
    return doc.toJson(QJsonDocument::Indented);
}

void TestCommandEventSpyEventSystem::handleEntityData(EventData *evData, QJsonObject &jsonEventInfo)
{
    EntityData *eData = static_cast<EntityData*>(evData);
    Q_ASSERT(eData != nullptr);
    jsonEventInfo.insert("EvData", "EntityData");
    jsonEventInfo.insert("EventCommand", strEntityCommand(eData->eventCommand()));
}

void TestCommandEventSpyEventSystem::handleComponentData(EventData *evData, QJsonObject &jsonEventInfo)
{
    ComponentData *cData = static_cast<ComponentData*>(evData);
    Q_ASSERT(cData != nullptr);
    jsonEventInfo.insert("EvData", "ComponentData");
    // TODO
}

void TestCommandEventSpyEventSystem::handleIntrospectionData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    IntrospectionData *iData = static_cast<IntrospectionData*>(evData);
    Q_ASSERT(iData != nullptr);
    jsonEventInfo.insert("EvData", "IntrospectionData");
    QJsonObject jsonIntrospection = iData->jsonData();
    jsonIntrospection = QJsonObject::fromVariantMap(jsonIntrospection.toVariantMap());
    jsonEventInfo.insert("IntrospectionData", jsonIntrospection);
}

void TestCommandEventSpyEventSystem::handleRpcData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    RemoteProcedureData *rpcData = static_cast<RemoteProcedureData*>(evData);
    Q_ASSERT(rpcData != nullptr);
    jsonEventInfo.insert("EvData", "RemoteProcedureData");
    // TODO
}

void TestCommandEventSpyEventSystem::handleErrorData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    ErrorData *errData = static_cast<ErrorData*>(evData);
    Q_ASSERT(errData != nullptr);
    jsonEventInfo.insert("EvData", "ErrorData");
    // TODO
}

QJsonObject TestCommandEventSpyEventSystem::commonInfo(CommandEvent *cEvent, EventData *evData)
{
    QJsonObject jsonEntity( {
        {"Entity", evData->entityId()},
        {"EventSubtype", strSubtype(cEvent->eventSubtype())},
        {"EventOrigin", strOrigin(qint8(evData->eventOrigin()))},
        {"EventTarget", strTarget(qint8(evData->eventTarget()))},
        {"ValidPeer", !cEvent->peerId().isNull()},
        {"Valid", evData->isValid()},
    } );
    return jsonEntity;
}

void TestCommandEventSpyEventSystem::addJsonInfo(const QJsonObject &jsonEventInfo)
{
    QJsonArray jsonArray;
    if(!m_jsonEvents.isEmpty())
        jsonArray = m_jsonEvents["VeinEvents"].toArray();
    jsonArray.append(jsonEventInfo);
    m_jsonEvents["VeinEvents"] = jsonArray;
}

QString TestCommandEventSpyEventSystem::strSubtype(CommandEvent::EventSubtype subtype)
{
    QString str;
    switch(subtype) {
    case CommandEvent::EventSubtype::NOTIFICATION:
        str = "NOTIFICATION";
        break;
    case CommandEvent::EventSubtype::TRANSACTION:
        str = "TRANSACTION";
        break;
    }
    return str;
}

QString TestCommandEventSpyEventSystem::strOrigin(qint8 origin)
{
    QString str;
    switch(origin) {
    case qint8(EventData::EventOrigin::EO_LOCAL):
        str = "EO_LOCAL";
        break;
    case qint8(EventData::EventOrigin::EO_FOREIGN):
        str = "EO_FOREIGN";
        break;
    default:
        str = QString().arg("EO_USER_DEFINED + %1", origin - qint8(EventData::EventOrigin::EO_USER_DEFINED));
        break;
    }
    return str;
}

QString TestCommandEventSpyEventSystem::strTarget(qint8 target)
{
    QString str;
    switch(target) {
    case qint8(EventData::EventTarget::ET_IRRELEVANT):
        str = "ET_IRRELEVANT";
        break;
    case qint8(EventData::EventTarget::ET_LOCAL):
        str = "ET_LOCAL";
        break;
    case qint8(EventData::EventTarget::ET_ALL):
        str = "ET_ALL";
        break;
    default:
        str = QString().arg("ET_USER_DEFINED + %1", target - qint8(EventData::EventTarget::ET_USER_DEFINED));
        break;
    }
    return str;
}

QString TestCommandEventSpyEventSystem::strEntityCommand(VeinComponent::EntityData::Command cmd)
{
    QString str;
    switch(cmd) {
    case EntityData::Command::ECMD_INVALID:
        str = "ECMD_INVALID";
        break;
    case EntityData::Command::ECMD_ADD:
        str = "ECMD_ADD";
        break;
    case EntityData::Command::ECMD_REMOVE:
        str = "ECMD_REMOVE";
        break;
    case EntityData::Command::ECMD_SUBSCRIBE:
        str = "ECMD_SUBSCRIBE";
        break;
    case EntityData::Command::ECMD_UNSUBSCRIBE:
        str = "ECMD_UNSUBSCRIBE";
        break;
    }
    return str;
}
