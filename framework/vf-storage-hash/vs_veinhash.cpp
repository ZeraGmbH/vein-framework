#include "vs_veinhash.h"

#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_errordata.h>
#include <ve_commandevent.h>
#include <QEvent>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include <QElapsedTimer>

Q_LOGGING_CATEGORY(VEIN_STORAGE_HASH, VEIN_DEBUGNAME_STORAGE_HASH)
Q_LOGGING_CATEGORY(VEIN_STORAGE_HASH_VERBOSE, VEIN_DEBUGNAME_STORAGE_HASH_VERBOSE)


using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinStorage
{
VeinHash::VeinHash(QObject *t_parent) :
    StorageSystem(t_parent)
{
    vCDebug(VEIN_STORAGE_HASH) << "Created VeinHash storage";
}

void VeinHash::setAcceptableOrigin(QList<EventData::EventOrigin> t_origins)
{
    m_acceptableOrigins = t_origins;
}

const QList<EventData::EventOrigin> &VeinHash::getAcceptableOrigin() const
{
    return m_acceptableOrigins;
}

VeinHash::~VeinHash()
{
    vCDebug(VEIN_STORAGE_HASH) << "Destroyed VeinHash storage";

    for(int i=0; i<m_data->count(); ++i)
    {
        QHash<QString, QVariant> *tmpToDelete = m_data->values().at(i);
        delete tmpToDelete;
    }
    delete m_data;
}


void VeinHash::processEvent(QEvent *t_event)
{
    if(t_event->type()==CommandEvent::eventType())
    {
        CommandEvent *cEvent = nullptr;
        EventData *evData = nullptr;
        cEvent = static_cast<CommandEvent *>(t_event);
        Q_ASSERT(cEvent != nullptr);

        evData = cEvent->eventData();
        Q_ASSERT(evData != nullptr);

        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION && m_acceptableOrigins.contains(evData->eventOrigin()))
        {
            switch (evData->type())
            {
            case ComponentData::dataType():
            {
                ComponentData *cData=nullptr;
                cData = static_cast<ComponentData *>(evData);
                Q_ASSERT(cData != nullptr);

                if(Q_UNLIKELY(cData->newValue().isValid() == false && cData->eventCommand() == ComponentData::Command::CCMD_SET))
                {
                    vCDebug(VEIN_STORAGE_HASH) << "Dropping event (command = CCMD_SET) with invalid event data:\nComponent name:" << cData->componentName() << "Value:" << cData->newValue();
                    t_event->accept();
                }
                else
                {
                    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Processing component data from event" << cEvent;
                    processComponentData(cData);
                }
                break;
            }
            case EntityData::dataType():
            {
                EntityData *eData=nullptr;
                eData = static_cast<EntityData *>(evData);
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
    QElapsedTimer qET;
    qET.start();
    if( (outputFileDevice->isOpen() || outputFileDevice->open(QFile::WriteOnly)) &&
         outputFileDevice->isWritable()) {

        QJsonDocument tmpDoc;
        QJsonObject rootObject;

        QList<int> tmpEntityIdKeys = m_data->keys();
        std::sort(tmpEntityIdKeys.begin(), tmpEntityIdKeys.end());
        for(const int tmpEntityId : tmpEntityIdKeys) {
            if(!entityFilter.isEmpty() && !entityFilter.contains(tmpEntityId))
                continue;
            const QHash<QString, QVariant>* entityHashPointer = m_data->value(tmpEntityId);
            QJsonObject tmpEntityObject;

            QStringList tmpEntityComponentNames = m_data->value(tmpEntityId)->keys();
            std::sort(tmpEntityComponentNames.begin(), tmpEntityComponentNames.end());
            for(const QString &tmpComponentName : tmpEntityComponentNames) {
                QVariant tmpData = entityHashPointer->value(tmpComponentName);
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
        tmpDoc.setObject(rootObject);
        outputFileDevice->write(tmpDoc.toJson());
    }

    if(outputFileDevice->isOpen())
        outputFileDevice->close();
    vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Dump finished in" << qET.nsecsElapsed() << "nSecs" << qET.elapsed();
}


QVariant VeinHash::getStoredValue(int t_entityId, const QString &t_componentName) const
{
    QVariant retVal;
    if(m_data->contains(t_entityId))
    {
        retVal = m_data->value(t_entityId)->value(t_componentName);
    }
    else
    {
        qCWarning(VEIN_STORAGE_HASH) << "Unknown entity with id:" <<  t_entityId << "component" << t_componentName;
    }
    return retVal;
}

bool VeinHash::hasStoredValue(int t_entityId, const QString &t_componentName) const
{
    return m_data->contains(t_entityId) && m_data->value(t_entityId)->contains(t_componentName);
}


bool VeinHash::initializeData(const QUrl &t_sourceUrl)
{
    Q_UNUSED(t_sourceUrl)
    qCWarning(VEIN_STORAGE_HASH) << "Function initializeData is currently not implemented";
    Q_ASSERT(false);
    return false;
}

QList<QString> VeinHash::getEntityComponents(int t_entityId) const
{
    return m_data->value(t_entityId)->keys();
}

bool VeinHash::hasEntity(int t_entityId) const
{
    return m_data->contains(t_entityId);
}

QList<int> VeinHash::getEntityList() const
{
    return m_data->keys();
}

bool VeinHash::processComponentData(ComponentData *t_cData)
{
    Q_ASSERT(t_cData != nullptr);

    bool retVal=false;
    const QString componentName= t_cData->componentName();
    switch(t_cData->eventCommand())
    {
    case ComponentData::Command::CCMD_ADD:
    {
        if(m_data->contains(t_cData->entityId()) == false)
        {
            QString tmpErrorString = tr("can not add value for invalid entity id: %1").arg(t_cData->entityId());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, t_cData);
        }
        else if(m_data->value(t_cData->entityId())->contains(componentName) == true)
        {
            QString tmpErrorString = tr("value already exists for component: %1 %2").arg(t_cData->entityId()).arg(t_cData->componentName());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, t_cData);
        }
        else
        {
            vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "adding component:" << t_cData->entityId() << componentName << "with value:" << t_cData->newValue();
            m_data->value(t_cData->entityId())->insert(componentName,t_cData->newValue());
            retVal = true;
        }
        break;
    }

    case ComponentData::Command::CCMD_REMOVE:
    {
        if(m_data->contains(t_cData->entityId()) && m_data->value(t_cData->entityId())->contains(componentName))
        {
            vCDebug(VEIN_STORAGE_HASH) << "removing entry:" << t_cData->entityId() << componentName;
            m_data->value(t_cData->entityId())->remove(componentName);
            retVal = true;
        }
        break;
    }
    case ComponentData::Command::CCMD_SET:
    {
        if(m_data->contains(t_cData->entityId()) == false)
        {
            QString tmpErrorString = tr("can not set value for nonexistant entity id: %1").arg(t_cData->entityId());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, t_cData);
        }
        else if(m_data->value(t_cData->entityId())->contains(componentName) == false)
        {
            QString tmpErrorString = tr("can not set value for nonexistant component: %1 %2").arg(t_cData->entityId()).arg(t_cData->componentName());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, t_cData);
        }
        else
        {
            vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "setting key:" << componentName << "from:" << m_data->value(t_cData->entityId())->value(componentName) << "to:" << t_cData->newValue();
            m_data->value(t_cData->entityId())->insert(componentName,t_cData->newValue());
            retVal = true;
        }
        break;
    }
    case ComponentData::Command::CCMD_FETCH:
    {
        ///@todo @bug remove inconsistent behavior by sending a new event instead of rewriting the current event
        vCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Processing CCMD_FETCH, entityId:" << t_cData->entityId() << "componentName:" << componentName;
        t_cData->setNewValue(getStoredValue(t_cData->entityId(), componentName));
        t_cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
        t_cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
        retVal = true;
        break;
    }
    default:
        break;
    }
    return retVal;
}

bool VeinHash::processEntityData(EntityData *t_eData)
{
    Q_ASSERT(t_eData != nullptr);

    bool retVal =false;
    switch(t_eData->eventCommand())
    {
    case EntityData::Command::ECMD_ADD:
    {
        if(m_data->contains(t_eData->entityId()) == false)
        {
            QHash<QString, QVariant> *tmpHash = new QHash<QString, QVariant>();
            m_data->insert(t_eData->entityId(), tmpHash);
            retVal = true;
        }
        else
        {
            QString tmpErrorString = tr("Cannot add entity, entity id already exists: %1").arg(t_eData->entityId());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, t_eData);
        }
        break;
    }

    case EntityData::Command::ECMD_REMOVE:
    {
        if(m_data->contains(t_eData->entityId()) == true)
        {
            QHash<QString, QVariant> *tmpHash = m_data->value(t_eData->entityId());
            m_data->remove(t_eData->entityId());
            delete tmpHash;
            retVal = true;
        }
        else
        {
            QString tmpErrorString = tr("Cannot delete entity, entity id does not exists: %1").arg(t_eData->entityId());
            qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
            sendError(tmpErrorString, t_eData);
        }
        break;
    }

    default: //ECMD_SUBSCRIBE etc. is handled by the networksystem
        break;
    }
    return retVal;
}

void VeinHash::sendError(const QString &t_errorString, EventData *t_data)
{
    Q_ASSERT(t_data != nullptr);

    ErrorData *errData = new ErrorData();

    errData->setEntityId(t_data->entityId());
    errData->setOriginalData(t_data);
    errData->setEventOrigin(EventData::EventOrigin::EO_LOCAL);
    errData->setEventTarget(t_data->eventTarget());
    errData->setErrorDescription(t_errorString);

    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, errData);
    emit sigSendEvent(cEvent);
}
}
