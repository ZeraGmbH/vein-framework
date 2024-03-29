#include "testcommandeventspyeventsystem.h"
#include "testcommandeventstrings.h"
#include <vcmp_errordata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_remoteproceduredata.h>
#include <QJsonArray>

using namespace VeinEvent;
using namespace VeinComponent;

TestCommandEventSpyEventSystem::TestCommandEventSpyEventSystem(QJsonObject *jsonEvents, QString roleName) :
    m_jsonEvents(jsonEvents),
    m_roleName(roleName)
{
}

void TestCommandEventSpyEventSystem::processEvent(QEvent *event)
{
    if(event->type() == CommandEvent::getQEventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        Q_ASSERT(cEvent != nullptr);
        EventData *evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);
        QJsonObject jsonEventInfo = baseInfoFromEventData(evData);
        extendByEventInfo(cEvent, jsonEventInfo);

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
   *m_jsonEvents = QJsonObject();
}

bool TestCommandEventSpyEventSystem::isEmpty() const
{
   return m_jsonEvents->isEmpty();
}

void TestCommandEventSpyEventSystem::handleEntityData(EventData *evData, QJsonObject &jsonEventInfo)
{
    EntityData *eData = static_cast<EntityData*>(evData);
    Q_ASSERT(eData != nullptr);
    jsonEventInfo.insert("Attached data", "EntityData");
    jsonEventInfo.insert("EntityCommand", TestCommandEventStrings::strEntityCommand(eData->eventCommand()));
}

void TestCommandEventSpyEventSystem::handleComponentData(EventData *evData, QJsonObject &jsonEventInfo)
{
    ComponentData *cData = static_cast<ComponentData*>(evData);
    Q_ASSERT(cData != nullptr);
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

void TestCommandEventSpyEventSystem::handleIntrospectionData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    IntrospectionData *iData = static_cast<IntrospectionData*>(evData);
    Q_ASSERT(iData != nullptr);
    jsonEventInfo.insert("Attached data", "IntrospectionData");
    QJsonObject jsonIntrospection = iData->jsonData();
    jsonIntrospection = QJsonObject::fromVariantMap(jsonIntrospection.toVariantMap());
    jsonEventInfo.insert("IntrospectionData", jsonIntrospection);
}

void TestCommandEventSpyEventSystem::handleRpcData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    RemoteProcedureData *rpcData = static_cast<RemoteProcedureData*>(evData);
    Q_ASSERT(rpcData != nullptr);
    jsonEventInfo.insert("Attached data", "RemoteProcedureData");
    // TODO
}

void TestCommandEventSpyEventSystem::handleErrorData(VeinEvent::EventData *evData, QJsonObject &jsonEventInfo)
{
    ErrorData *errData = static_cast<ErrorData*>(evData);
    Q_ASSERT(errData != nullptr);
    jsonEventInfo.insert("Attached data", "ErrorData");

    QJsonObject origEventInfo = baseInfoFromEventData(errData);
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

void TestCommandEventSpyEventSystem::extendByEventInfo(CommandEvent *cEvent, QJsonObject &jsonInfo)
{
    jsonInfo.insert("Am at", m_roleName);
    jsonInfo.insert("EventSubtype", TestCommandEventStrings::strSubtype(cEvent->eventSubtype()));
    jsonInfo.insert("ValidPeer", !cEvent->peerId().isNull());
}

QJsonObject TestCommandEventSpyEventSystem::baseInfoFromEventData(VeinEvent::EventData *evData)
{
    QJsonObject jsonEntity( {
        {"Entity", evData->entityId()},
        {"EventOrigin", TestCommandEventStrings::strOrigin(qint8(evData->eventOrigin()))},
        {"EventTarget", TestCommandEventStrings::strTarget(qint8(evData->eventTarget()))},
        {"Valid", evData->isValid()},
    } );
    return jsonEntity;
}

void TestCommandEventSpyEventSystem::addJsonInfo(const QJsonObject &jsonEventInfo)
{
    QJsonArray jsonArray;
    if(!m_jsonEvents->isEmpty())
        jsonArray = (*m_jsonEvents)["VeinEvents"].toArray();
    jsonArray.append(jsonEventInfo);
    (*m_jsonEvents)["VeinEvents"] = jsonArray;
}
