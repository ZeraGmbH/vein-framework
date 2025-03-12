#include "testjsonspyeventsystem.h"
#include "testcommandeventstrings.h"
#include <vcmp_errordata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_remoteproceduredata.h>
#include <QJsonArray>

using namespace VeinEvent;
using namespace VeinComponent;
using namespace VeinNet;

TestJsonSpyEventSystem::TestJsonSpyEventSystem(QJsonObject *jsonEvents, QString roleName, bool handleProtocolEvents) :
    m_jsonEvents(jsonEvents),
    m_roleName(roleName),
    m_handleProtocolEvents(handleProtocolEvents)
{
}

void TestJsonSpyEventSystem::processEvent(QEvent *event)
{
    QJsonObject jsonEventInfo = eventToJsonInfo(event);
    if(!jsonEventInfo.isEmpty())
        addJsonInfo(jsonEventInfo);
}

void TestJsonSpyEventSystem::clear()
{
   *m_jsonEvents = QJsonObject();
}

bool TestJsonSpyEventSystem::isEmpty() const
{
   return m_jsonEvents->isEmpty();
}

void TestJsonSpyEventSystem::onEventAccepted(EventSystem *eventSystem, QEvent *event)
{
   QJsonObject jsonEventInfo = eventToJsonInfo(event);
   jsonEventInfo.insert("Accepted (finalized) by", eventSystem->metaObject()->className());
   addJsonInfo(jsonEventInfo);
}

QJsonObject TestJsonSpyEventSystem::eventToJsonInfo(QEvent *event)
{
   QJsonObject jsonEventInfo;
   if(event->type() == CommandEvent::getQEventType()) {
       CommandEvent *cEvent = static_cast<CommandEvent*>(event);
       EventData *evData = cEvent->eventData();
       baseInfoFromEventData(evData, jsonEventInfo);
       extendByCommandEventInfo(cEvent, jsonEventInfo);

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
   }
   else if(m_handleProtocolEvents && event->type() == ProtocolEvent::getQEventType()) {
       ProtocolEvent *pEvent = static_cast<ProtocolEvent*>(event);
       handleProtocolEvent(pEvent, jsonEventInfo);
   }
   return jsonEventInfo;
}

void TestJsonSpyEventSystem::handleEntityData(EventData *evData, QJsonObject &jsonEventInfo)
{
    EntityData *eData = static_cast<EntityData*>(evData);
    jsonEventInfo.insert("Attached data", "EntityData");
    jsonEventInfo.insert("EntityCommand", TestCommandEventStrings::strEntityCommand(eData->eventCommand()));
}

void TestJsonSpyEventSystem::handleComponentData(EventData *evData, QJsonObject &jsonEventInfo)
{
    ComponentData *cData = static_cast<ComponentData*>(evData);
    jsonEventInfo.insert("Attached data", "ComponentData");
    jsonEventInfo.insert("ComponentName", cData->componentName());
    jsonEventInfo.insert("ComponentCommand", TestCommandEventStrings::strComponentCommand(cData->eventCommand()));
    QVariant oldValue = cData->oldValue();
    if(oldValue.isValid())
        jsonEventInfo.insert("ValueOld", oldValue.toJsonValue());
    else
        jsonEventInfo.insert("ValueOld", "invalid");
    QVariant newValue = cData->newValue();
    if(newValue.isValid())
        jsonEventInfo.insert("ValueNew", newValue.toJsonValue());
    else
        jsonEventInfo.insert("ValueNew", "invalid");
}

void TestJsonSpyEventSystem::handleIntrospectionData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    IntrospectionData *iData = static_cast<IntrospectionData*>(evData);
    jsonEventInfo.insert("Attached data", "IntrospectionData");
    QJsonObject jsonIntrospection = iData->jsonData();
    jsonIntrospection = QJsonObject::fromVariantMap(jsonIntrospection.toVariantMap());
    jsonEventInfo.insert("IntrospectionData", jsonIntrospection);
}

void TestJsonSpyEventSystem::handleRpcData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    RemoteProcedureData *rpcData = static_cast<RemoteProcedureData*>(evData);
    jsonEventInfo.insert("Attached data", "RemoteProcedureData");
    jsonEventInfo.insert("ProcedureName", rpcData->procedureName());
    jsonEventInfo.insert("ComponentCommand", TestCommandEventStrings::strRPCCommand(rpcData->command()));
}

void TestJsonSpyEventSystem::handleErrorData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    ErrorData *errData = static_cast<ErrorData*>(evData);
    jsonEventInfo.insert("Attached data", "ErrorData");

    QJsonObject origEventInfo;
    baseInfoFromEventData(errData, origEventInfo);
    int originalDataType = errData->originalDataType();
    switch (originalDataType) {
    case VCMP_COMPONENTDATA_DATATYPE: {
        ComponentData data;
        data.deserialize(errData->originalData());
        handleComponentData(&data, origEventInfo);
        break;
    }
    case VCMP_ENTITYDATA_DATATYPE: {
        EntityData data;
        data.deserialize(errData->originalData());
        handleEntityData(&data, origEventInfo);
        break;
    }
    case VCMP_INTROSPECTIONDATA_DATATYPE: {
        IntrospectionData data;
        data.deserialize(errData->originalData());
        handleIntrospectionData(&data, origEventInfo);
        break;
    }
    case VCMP_ERRORDATA_DATATYPE: // cannot be nested see ErrorData::setOriginalData
        break;
    case VCMP_REMOTEPROCEDUREDATA_DATATYPE: {
        RemoteProcedureData data;
        data.deserialize(errData->originalData());
        handleRpcData(&data, origEventInfo);
        break;
    }
    }
    jsonEventInfo.insert("OriginalEventData", origEventInfo);
}

void TestJsonSpyEventSystem::handleProtocolEvent(ProtocolEvent *pEvent, QJsonObject &jsonEventInfo)
{
    jsonEventInfo.insert("Am protocol event in", m_roleName);
    jsonEventInfo.insert("ReceiversCount", pEvent->receivers().count());
    jsonEventInfo.insert("LocalOrigin", pEvent->isOfLocalOrigin());
    jsonEventInfo.insert("ValidPeer", !pEvent->peerId().isNull());
}

void TestJsonSpyEventSystem::extendByCommandEventInfo(CommandEvent *cEvent, QJsonObject &jsonEventInfo)
{
    jsonEventInfo.insert("Am command event in", m_roleName);
    jsonEventInfo.insert("EventSubtype", TestCommandEventStrings::strSubtype(cEvent->eventSubtype()));
    jsonEventInfo.insert("ValidPeer", !cEvent->peerId().isNull());
}

void TestJsonSpyEventSystem::baseInfoFromEventData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    jsonEventInfo.insert("Entity", evData->entityId());
    jsonEventInfo.insert("EventOrigin", TestCommandEventStrings::strOrigin(qint8(evData->eventOrigin())));
    jsonEventInfo.insert("EventTarget", TestCommandEventStrings::strTarget(qint8(evData->eventTarget())));
    jsonEventInfo.insert("Valid", evData->isValid());
}

void TestJsonSpyEventSystem::addJsonInfo(const QJsonObject &jsonEventInfo)
{
    QJsonArray jsonArray;
    if(!m_jsonEvents->isEmpty())
        jsonArray = (*m_jsonEvents)["VeinEvents"].toArray();
    jsonArray.append(jsonEventInfo);
    (*m_jsonEvents)["VeinEvents"] = jsonArray;
}
