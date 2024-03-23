#include "vs_veinhash.h"
#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_errordata.h>
#include <ve_commandevent.h>
#include <QEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Q_LOGGING_CATEGORY(VEIN_STORAGE_HASH, VEIN_DEBUGNAME_STORAGE_HASH)
Q_LOGGING_CATEGORY(VEIN_STORAGE_HASH_VERBOSE, VEIN_DEBUGNAME_STORAGE_HASH_VERBOSE)

using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinStorage
{
VeinHash::VeinHash(QObject *parent) :
    StorageSystem(parent)
{
}

void VeinHash::setAcceptableOrigin(QList<EventData::EventOrigin> origins)
{
    m_acceptableOrigins = origins;
}

const QList<EventData::EventOrigin> &VeinHash::getAcceptableOrigin() const
{
    return m_acceptableOrigins;
}

void VeinHash::processEvent(QEvent *event)
{
    if(event->type()==CommandEvent::eventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        Q_ASSERT(cEvent != nullptr);
        EventData *evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);

        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION && m_acceptableOrigins.contains(evData->eventOrigin())) {
            switch (evData->type())
            {
            case ComponentData::dataType():
            {
                ComponentData *cData = static_cast<ComponentData *>(evData);
                Q_ASSERT(cData != nullptr);
                if(Q_UNLIKELY(cData->newValue().isValid() == false && cData->eventCommand() == ComponentData::Command::CCMD_SET)) {
                    vCDebug(VEIN_STORAGE_HASH) << "Dropping event (command = CCMD_SET) with invalid event data:\nComponent name:" << cData->componentName() << "Value:" << cData->newValue();
                    event->accept();
                }
                else {
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Processing component data from event" << cEvent;
                    processComponentData(cData);
                }
                break;
            }
            case EntityData::dataType():
            {
                EntityData *eData = static_cast<EntityData *>(evData);
                Q_ASSERT(eData != nullptr);

                vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Processing entity data from event" << cEvent;
                processEntityData(eData);
                break;
            }
            default:
                break;
            }
        }
    }
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
                vCDebug(VEIN_STORAGE_HASH_VERBOSE) << tmpData.typeName() << tmpDataType << QMetaType::type("QList<QString>") << QMetaType::type(tmpData.typeName());
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
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "inserted QJsonArray from QList<Int>" << tmpComponentName << tmpIntList;
                }
                else if(tmpDataType == QMetaType::type("QList<double>"))  { //needs manual conversion
                    QVariantList tmpDoubleList;
                    const auto doubleList = tmpData.value<QList<double> >();
                    for(const double tmpDouble : doubleList)
                        tmpDoubleList.append(tmpDouble);
                    toInsert = QJsonArray::fromVariantList(tmpDoubleList);
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "inserted QJsonArray from QList<double>" << tmpComponentName << tmpDoubleList;
                }
                else if(tmpDataType == QMetaType::QStringList) { //needs manual conversion
                    QVariantList tmpStringList;
                    const auto stringList = tmpData.value<QStringList>();
                    for(const QString &tmpString : stringList)
                        tmpStringList.append(tmpString);
                    toInsert = QJsonArray::fromVariantList(tmpStringList);
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "inserted QJsonArray from QList<QString>" << tmpComponentName << tmpStringList << stringList;
                }
                else if(tmpData.canConvert(QMetaType::QVariantList) && tmpData.toList().isEmpty() == false) {
                    toInsert = QJsonArray::fromVariantList(tmpData.toList());
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "inserted QJsonArray" << tmpComponentName << tmpData.toList();
                }
                else if(tmpData.canConvert(QMetaType::QVariantMap) && tmpData.toMap().isEmpty() == false) {
                    toInsert = QJsonObject::fromVariantMap(tmpData.toMap());
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "inserted QJsonObject" << tmpComponentName << tmpData.toMap();
                }
                else {
                    if(tmpComponentName == "INF_ModuleInterface" ) {
                        QJsonDocument doc = QJsonDocument::fromJson(tmpData.toString().toUtf8());
                        toInsert = doc.object();
                    }
                    else
                        toInsert = QJsonValue::fromVariant(tmpData);
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "inserted QJsonValue" << tmpComponentName << QJsonValue::fromVariant(tmpData) << tmpData;
                }
                if(toInsert.isNull()) //how to consistently store and retrieve a QVector2D or QDateTime in JSON?
                    qCWarning(VEIN_STORAGE_HASH) << "Datatype" << tmpData.typeName() << "from" << tmpEntityId << tmpComponentName << "is not supported by function " << __PRETTY_FUNCTION__;
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


QVariant VeinHash::getStoredValue(int entityId, const QString &componentName) const
{
    QVariant retVal;
    if(m_entityComponentData.contains(entityId))
        retVal = m_entityComponentData.value(entityId).value(componentName);
    else
        qCWarning(VEIN_STORAGE_HASH) << "Unknown entity with id:" <<  entityId << "component" << componentName;
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

void VeinHash::processComponentData(ComponentData *cData)
{
    Q_ASSERT(cData != nullptr);
    const QString componentName= cData->componentName();
    const int entityId = cData->entityId();
    switch(cData->eventCommand())
    {
    case ComponentData::Command::CCMD_ADD:
    {
        if(!m_entityComponentData.contains(entityId)) {
            QString tmpErrorString = QString("can not add value for invalid entity id: %1").arg(entityId);
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, cData);
        }
        else if(m_entityComponentData.value(entityId).contains(componentName)) {
            QString tmpErrorString = QString("value already exists for component: %1 %2").arg(entityId).arg(cData->componentName());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, cData);
        }
        else {
            vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "adding component:" << entityId << componentName << "with value:" << cData->newValue();
            m_entityComponentData[entityId][componentName] = cData->newValue();
        }
        break;
    }
    case ComponentData::Command::CCMD_REMOVE:
    {
        if(m_entityComponentData.contains(entityId) && m_entityComponentData.value(entityId).contains(componentName)) {
            vCDebug(VEIN_STORAGE_HASH) << "removing entry:" << entityId << componentName;
            m_entityComponentData[entityId].remove(componentName);
        }
        break;
    }
    case ComponentData::Command::CCMD_SET:
    {
        if(!m_entityComponentData.contains(entityId)) {
            QString tmpErrorString = tr("can not set value for nonexistant entity id: %1").arg(entityId);
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, cData);
        }
        else if(!m_entityComponentData[entityId].contains(componentName)) {
            QString tmpErrorString = tr("can not set value for nonexistant component: %1 %2").arg(entityId).arg(cData->componentName());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, cData);
        }
        else {
            vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "setting key:" << componentName << "from:" << m_entityComponentData.value(entityId).value(componentName) << "to:" << cData->newValue();
            m_entityComponentData[entityId][componentName] = cData->newValue();
        }
        break;
    }
    case ComponentData::Command::CCMD_FETCH:
    {
        ///@todo @bug remove inconsistent behavior by sending a new event instead of rewriting the current event
        vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Processing CCMD_FETCH, entityId:" << entityId << "componentName:" << componentName;
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
    Q_ASSERT(eData != nullptr);
    switch(eData->eventCommand())
    {
    case EntityData::Command::ECMD_ADD:
    {
        if(!m_entityComponentData.contains(eData->entityId()))
            m_entityComponentData.insert(eData->entityId(), QHash<QString, QVariant>());
        else {
            QString tmpErrorString = tr("Cannot add entity, entity id already exists: %1").arg(eData->entityId());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, eData);
        }
        break;
    }
    case EntityData::Command::ECMD_REMOVE:
    {
        if(m_entityComponentData.contains(eData->entityId()))
            m_entityComponentData.remove(eData->entityId());
        else {
            QString tmpErrorString = tr("Cannot delete entity, entity id does not exists: %1").arg(eData->entityId());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, eData);
        }
        break;
    }
    default: //ECMD_SUBSCRIBE etc. is handled by the networksystem
        break;
    }
}

void VeinHash::sendError(const QString &errorString, EventData *data)
{
    Q_ASSERT(data != nullptr);

    ErrorData *errData = new ErrorData();
    errData->setEntityId(data->entityId());
    errData->setOriginalData(data);
    errData->setEventOrigin(EventData::EventOrigin::EO_LOCAL);
    errData->setEventTarget(data->eventTarget());
    errData->setErrorDescription(errorString);

    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, errData);
    emit sigSendEvent(cEvent);
}
}
