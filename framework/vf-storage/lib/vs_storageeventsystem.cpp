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

}
