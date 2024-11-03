#include "vs_storageeventsystem.h"
#include "vs_databasehash.h"
#include <vcmp_entitydata.h>
#include <vcmp_errordatasender.h>
#include <ve_commandevent.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinStorage
{

StorageEventSystem::StorageEventSystem(QObject *parent) :
    AbstractEventSystem(parent),
    m_privHash(new DatabaseHash)
{
}

StorageEventSystem::~StorageEventSystem()
{
    delete m_privHash;
}

void StorageEventSystem::processEvent(QEvent *event)
{
    if(event->type() == CommandEvent::getQEventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        EventData *evData = cEvent->eventData();
        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION && m_acceptableOrigins.contains(evData->eventOrigin())) {
            switch (evData->type())
            {
            case ComponentData::dataType():
            {
                // Why is this done here??
                ComponentData *cData = static_cast<ComponentData *>(evData);
                if(Q_UNLIKELY(cData->newValue().isValid() == false && cData->eventCommand() == ComponentData::Command::CCMD_SET)) {
                    qWarning() << "Dropping event (command = CCMD_SET) with invalid event data:\nComponent name:" << cData->componentName() << "Value:" << cData->newValue();
                    event->accept();
                }
                else
                    processComponentData(event);
                break;
            }
            case EntityData::dataType():
                processEntityData(event);
                break;
            default:
                break;
            }
        }
    }
}

AbstractDatabase *StorageEventSystem::getDb() const
{
    return m_privHash;
}

void StorageEventSystem::processComponentData(QEvent *event)
{
    CommandEvent *cEvent = static_cast<CommandEvent *>(event);
    ComponentData *cData = static_cast<ComponentData *>(cEvent->eventData());
    const QString componentName = cData->componentName();
    const int entityId = cData->entityId();
    EntityMap* entity = m_privHash->findEntity(entityId);
    StorageComponentPtr component = m_privHash->findComponent(entity, componentName);

    switch(cData->eventCommand())
    {
    case ComponentData::Command::CCMD_ADD: {
        StorageComponentPtr futureComponent = m_privHash->takeFutureComponent(entityId, componentName);
        if(futureComponent)
            m_privHash->insertFutureComponent(entityId, componentName, futureComponent, cData->newValue());
        else if(!entity)
            ErrorDataSender::errorOut(QString("Cannot add component for invalid entity id: %1").arg(entityId), event, this);
        else if(component)
            ErrorDataSender::errorOut(QString("Value already exists for component: %1 %2").arg(entityId).arg(cData->componentName()), event, this);
        else
            m_privHash->insertComponentValue(entity, componentName, cData->newValue());
        break;
    }
    case ComponentData::Command::CCMD_REMOVE:
        if(!entity)
            ErrorDataSender::errorOut(QString("Cannot remove component for invalid entity id: %1").arg(entityId), event, this);
        else if(!component)
            ErrorDataSender::errorOut(QString("Cannot remove not existing component: %1 %2").arg(entityId).arg(cData->componentName()), event, this);
        else
            m_privHash->removeComponentValue(entity, componentName);
        break;
    case ComponentData::Command::CCMD_SET:
        if(!entity)
            ErrorDataSender::errorOut(QString("Cannot set component for not existing entity id: %1").arg(entityId), event, this);
        else if(!component)
            ErrorDataSender::errorOut(QString("Cannot set not existing component: %1 %2").arg(entityId).arg(cData->componentName()), event, this);
        else {
            if(component->getValue().isValid() && component->getValue().type() != cData->newValue().type())
                qWarning("QVariant type change detected on entity %i / component %s: Old %s / new %s",
                         entityId, qPrintable(componentName),
                         qPrintable(component->getValue().typeName()), qPrintable(cData->newValue().typeName()));
            m_privHash->changeComponentValue(component, cData->newValue());
        }
        break;
    case ComponentData::Command::CCMD_FETCH:
        if(!entity)
            ErrorDataSender::errorOut(QString("Cannot fetch component for not existing entity id: %1").arg(entityId), event, this);
        else if(!component)
            ErrorDataSender::errorOut(QString("Cannot fetch not existing component: %1 %2").arg(entityId).arg(cData->componentName()), event, this);
        else {
            ///@todo @bug remove inconsistent behavior by sending a new event instead of rewriting the current event
            cData->setNewValue(component->getValue());
            cData->setEventOrigin(ComponentData::EventOrigin::EO_LOCAL);
            cData->setEventTarget(ComponentData::EventTarget::ET_ALL);
        }
        break;
    default:
        break;
    }
}

void StorageEventSystem::processEntityData(QEvent *event)
{
    CommandEvent *cEvent = static_cast<CommandEvent *>(event);
    EntityData *eData = static_cast<EntityData *>(cEvent->eventData());
    const int entityId = eData->entityId();
    EntityMap* entityMap = m_privHash->findEntity(entityId);
    switch(eData->eventCommand())
    {
    case EntityData::Command::ECMD_ADD:
        if(entityMap)
            ErrorDataSender::errorOut(QString("Cannot add entity, entity id already exists: %1").arg(eData->entityId()), event, this);
        else
            m_privHash->insertEntity(entityId);
        break;
    case EntityData::Command::ECMD_REMOVE:
    {
        if(!entityMap)
            ErrorDataSender::errorOut(QString("Cannot delete entity, entity id does not exists: %1").arg(eData->entityId()), event, this);
        else
            m_privHash->removeEntity(entityId);
        break;
    }
    default: //ECMD_SUBSCRIBE etc. is handled by the networksystem
        break;
    }
}

void StorageEventSystem::setAcceptableOrigin(QList<EventData::EventOrigin> origins)
{
    m_acceptableOrigins = origins;
}

QVariant StorageEventSystem::getStoredValue(int entityId, const QString &componentName) const
{
    StorageComponentPtr component = m_privHash->findComponent(entityId, componentName);
    if(component)
        return component->getValue();
    qWarning() << "Unknown entity with id:" <<  entityId << "component" << componentName;
    return QVariant();
}

bool StorageEventSystem::hasStoredValue(int entityId, const QString &componentName) const
{
    return m_privHash->findComponent(entityId, componentName) != nullptr;
}

bool StorageEventSystem::hasEntity(int entityId) const
{
    return m_privHash->findEntity(entityId) != nullptr;
}

void StorageEventSystem::dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter) const
{
    if((outputFileDevice->isOpen() || outputFileDevice->open(QIODevice::WriteOnly)) &&
        outputFileDevice->isWritable()) {
        QJsonObject rootObject;
        QList<int> tmpEntityIdKeys = m_privHash->getEntityList();
        std::sort(tmpEntityIdKeys.begin(), tmpEntityIdKeys.end());
        for(const int tmpEntityId : qAsConst(tmpEntityIdKeys)) {
            if(!entityFilter.isEmpty() && !entityFilter.contains(tmpEntityId))
                continue;
            EntityMap* entityMap = m_privHash->findEntity(tmpEntityId);;
            QJsonObject tmpEntityObject;

            QStringList tmpEntityComponentNames = m_privHash->getComponentList(tmpEntityId);
            std::sort(tmpEntityComponentNames.begin(), tmpEntityComponentNames.end());
            for(const QString &tmpComponentName : tmpEntityComponentNames) {
                QVariant tmpData = m_privHash->findComponent(entityMap, tmpComponentName)->getValue();
                QJsonValue toInsert;
                int tmpDataType = QMetaType::type(tmpData.typeName());
                if(tmpDataType == QMetaType::type("QList<int>")) { //needs manual conversion
                    QVariantList tmpIntList;
                    auto intList = tmpData.value<QList<int> >();
                    if(tmpEntityId == 0 && tmpComponentName == "Entities")
                        std::sort(intList .begin(), intList .end());
                    else
                        qFatal("Unexpected int-list: Entity id: %i / comonente %s", tmpEntityId, qPrintable(tmpComponentName));
                    for(const int &tmpInt : qAsConst(intList))
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
                if(!tmpData.isNull() && toInsert.isNull()) //how to consistently store and retrieve a QVector2D or QDateTime in JSON?
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
