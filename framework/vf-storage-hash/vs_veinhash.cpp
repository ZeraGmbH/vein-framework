#include "vs_veinhash.h"
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_errordata.h>
#include <ve_commandevent.h>
#include <QEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinStorage
{
VeinHash::VeinHash(QObject *parent) :
    StorageSystem(parent)
{
}

void VeinHash::processEvent(QEvent *event)
{
    if(event->type()==CommandEvent::eventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        EventData *evData = cEvent->eventData();
        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION && m_acceptableOrigins.contains(evData->eventOrigin())) {
            switch (evData->type())
            {
            case ComponentData::dataType():
            {
                ComponentData *cData = static_cast<ComponentData *>(evData);
                if(Q_UNLIKELY(cData->newValue().isValid() == false && cData->eventCommand() == ComponentData::Command::CCMD_SET)) {
                    qWarning() << "Dropping event (command = CCMD_SET) with invalid event data:\nComponent name:" << cData->componentName() << "Value:" << cData->newValue();
                    event->accept();
                }
                else
                    processComponentData(cData);
                break;
            }
            case EntityData::dataType():
            {
                EntityData *eData = static_cast<EntityData *>(evData);
                processEntityData(eData);
                break;
            }
            default:
                break;
            }
        }
    }
}

void VeinHash::processComponentData(ComponentData *cData)
{
    const QString componentName= cData->componentName();
    const int entityId = cData->entityId();
    switch(cData->eventCommand())
    {
    case ComponentData::Command::CCMD_ADD:
    {
        if(!m_entityComponentData.contains(entityId))
            sendError(QString("can not add value for invalid entity id: %1").arg(entityId), cData);
        else if(m_entityComponentData.value(entityId).contains(componentName))
            sendError(QString("value already exists for component: %1 %2").arg(entityId).arg(cData->componentName()), cData);
        else
            m_entityComponentData[entityId].insert(componentName, cData->newValue());
        break;
    }
    case ComponentData::Command::CCMD_REMOVE:
    {
        if(m_entityComponentData.contains(entityId) && m_entityComponentData.value(entityId).contains(componentName))
            m_entityComponentData[entityId].remove(componentName);
        break;
    }
    case ComponentData::Command::CCMD_SET:
    {
        if(!m_entityComponentData.contains(entityId))
            sendError(QString("can not set value for nonexistant entity id: %1").arg(entityId), cData);
        else if(!m_entityComponentData[entityId].contains(componentName))
            sendError(QString("can not set value for nonexistant component: %1 %2").arg(entityId).arg(cData->componentName()), cData);
        else
            m_entityComponentData[entityId][componentName] = cData->newValue();
        break;
    }
    case ComponentData::Command::CCMD_FETCH:
    {
        ///@todo @bug remove inconsistent behavior by sending a new event instead of rewriting the current event
        cData->setNewValue(getStoredValue(entityId, componentName));
        cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
        cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
        break;
    }
    default:
        break;
    }
}

void VeinHash::processEntityData(EntityData *eData)
{
    switch(eData->eventCommand())
    {
    case EntityData::Command::ECMD_ADD:
    {
        if(!m_entityComponentData.contains(eData->entityId()))
            m_entityComponentData.insert(eData->entityId(), QHash<QString, QVariant>());
        else
            sendError(QString("Cannot add entity, entity id already exists: %1").arg(eData->entityId()), eData);
        break;
    }
    case EntityData::Command::ECMD_REMOVE:
    {
        if(m_entityComponentData.contains(eData->entityId()))
            m_entityComponentData.remove(eData->entityId());
        else
            sendError(QString("Cannot delete entity, entity id does not exists: %1").arg(eData->entityId()), eData);
        break;
    }
    default: //ECMD_SUBSCRIBE etc. is handled by the networksystem
        break;
    }
}

void VeinHash::setAcceptableOrigin(QList<EventData::EventOrigin> origins)
{
    m_acceptableOrigins = origins;
}

const QList<EventData::EventOrigin> &VeinHash::getAcceptableOrigin() const
{
    return m_acceptableOrigins;
}

QVariant VeinHash::getStoredValue(int entityId, const QString &componentName) const
{
    QVariant retVal;
    if(m_entityComponentData.contains(entityId))
        retVal = m_entityComponentData.value(entityId).value(componentName);
    else
        qWarning() << "Unknown entity with id:" <<  entityId << "component" << componentName;
    return retVal;
}

bool VeinHash::hasStoredValue(int entityId, const QString &componentName) const
{
    return m_entityComponentData.contains(entityId) && m_entityComponentData.value(entityId).contains(componentName);
}


QList<QString> VeinHash::getEntityComponents(int entityId) const
{
    return m_entityComponentData.value(entityId).keys();
}

bool VeinHash::hasEntity(int entityId) const
{
    return m_entityComponentData.contains(entityId);
}

QList<int> VeinHash::getEntityList() const
{
    return m_entityComponentData.keys();
}

void VeinHash::sendError(const QString &errorString, EventData *data)
{
    qWarning() << errorString;
    ErrorData *errData = new ErrorData();
    errData->setEntityId(data->entityId());
    errData->setOriginalData(data);
    errData->setEventOrigin(EventData::EventOrigin::EO_LOCAL);
    errData->setEventTarget(data->eventTarget());
    errData->setErrorDescription(errorString);

    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, errData);
    emit sigSendEvent(cEvent);
}

void VeinHash::dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter) const
{
    if((outputFileDevice->isOpen() || outputFileDevice->open(QIODevice::WriteOnly)) &&
        outputFileDevice->isWritable()) {
        QJsonObject rootObject;
        QList<int> tmpEntityIdKeys = m_entityComponentData.keys();
        std::sort(tmpEntityIdKeys.begin(), tmpEntityIdKeys.end());
        for(const int tmpEntityId : tmpEntityIdKeys) {
            if(!entityFilter.isEmpty() && !entityFilter.contains(tmpEntityId))
                continue;
            const QHash<QString, QVariant> entityHashPointer = m_entityComponentData.value(tmpEntityId);
            QJsonObject tmpEntityObject;

            QStringList tmpEntityComponentNames = m_entityComponentData.value(tmpEntityId).keys();
            std::sort(tmpEntityComponentNames.begin(), tmpEntityComponentNames.end());
            for(const QString &tmpComponentName : tmpEntityComponentNames) {
                QVariant tmpData = entityHashPointer.value(tmpComponentName);
                QJsonValue toInsert;
                int tmpDataType = QMetaType::type(tmpData.typeName());
                if(tmpDataType == QMetaType::type("QList<int>")) { //needs manual conversion
                    QVariantList tmpIntList;
                    auto intList = tmpData.value<QList<int> >();
                    if(tmpEntityId == 0 && tmpComponentName == "Entities")
                        std::sort(intList .begin(), intList .end());
                    else
                        qFatal("Unexpected int-list: Entity id: %i / comonente %s", tmpEntityId, qPrintable(tmpComponentName));
                    for(const int &tmpInt : intList)
                        tmpIntList.append(tmpInt);
                    toInsert = QJsonArray::fromVariantList(tmpIntList);
                }
                else if(tmpDataType == QMetaType::type("QList<double>"))  { //needs manual conversion
                    QVariantList tmpDoubleList;
                    const auto doubleList = tmpData.value<QList<double> >();
                    for(const double tmpDouble : doubleList)
                        tmpDoubleList.append(tmpDouble);
                    toInsert = QJsonArray::fromVariantList(tmpDoubleList);
                }
                else if(tmpDataType == QMetaType::QStringList) { //needs manual conversion
                    QVariantList tmpStringList;
                    const auto stringList = tmpData.value<QStringList>();
                    for(const QString &tmpString : stringList)
                        tmpStringList.append(tmpString);
                    toInsert = QJsonArray::fromVariantList(tmpStringList);
                }
                else if(tmpData.canConvert(QMetaType::QVariantList) && tmpData.toList().isEmpty() == false)
                    toInsert = QJsonArray::fromVariantList(tmpData.toList());
                else if(tmpData.canConvert(QMetaType::QVariantMap) && tmpData.toMap().isEmpty() == false)
                    toInsert = QJsonObject::fromVariantMap(tmpData.toMap());
                else {
                    if(tmpComponentName == "INF_ModuleInterface" ) {
                        QJsonDocument doc = QJsonDocument::fromJson(tmpData.toString().toUtf8());
                        toInsert = doc.object();
                    }
                    else
                        toInsert = QJsonValue::fromVariant(tmpData);
                }
                if(toInsert.isNull()) //how to consistently store and retrieve a QVector2D or QDateTime in JSON?
                    qWarning() << "Datatype" << tmpData.typeName() << "from" << tmpEntityId << tmpComponentName << "is not supported by function " << __PRETTY_FUNCTION__;
                tmpEntityObject.insert(tmpComponentName, toInsert);
            }
            rootObject.insert(QString::number(tmpEntityId), tmpEntityObject);
        }
        QJsonDocument tmpDoc;
        tmpDoc.setObject(rootObject);
        outputFileDevice->write(tmpDoc.toJson());
    }

    if(outputFileDevice->isOpen())
        outputFileDevice->close();
}

}
