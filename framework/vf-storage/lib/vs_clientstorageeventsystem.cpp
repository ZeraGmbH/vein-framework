#include "vs_clientstorageeventsystem.h"
#include <vcmp_entitydata.h>
#include <ve_commandevent.h>
#include <vcmp_componentdata.h>
#include <vcmp_introspectiondata.h>
#include <vcmp_errordatasender.h>

using namespace VeinEvent;
using namespace VeinComponent;
using namespace VeinStorage;

ClientStorageEventSystem::ClientStorageEventSystem(QObject *parent) :
    AbstractEventSystem{parent},
    m_privHash(new DatabaseHash)
{}

AbstractDatabase *ClientStorageEventSystem::getDb() const
{
    return m_privHash;
}

void ClientStorageEventSystem::processEvent(QEvent *event)
{
    if(event->type() == VeinEvent::CommandEvent::getQEventType()) {
        CommandEvent *cEvent = static_cast<CommandEvent *>(event);
        EventData *evData = cEvent->eventData();
        if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION && m_acceptableOrigins.contains(evData->eventOrigin())) {
            switch (evData->type())
            {
            case IntrospectionData::dataType():
                processIntrospectionData(event);
                break;
            case ComponentData::dataType():
                processComponentData(event);
                break;
            case EntityData::dataType():
                processEntityData(event);
                break;
            default:
                break;
            }
        }
    }
}

void ClientStorageEventSystem::processIntrospectionData(QEvent *event)
{
    CommandEvent *cEvent = static_cast<CommandEvent *>(event);
    IntrospectionData *iData = static_cast<IntrospectionData *>(cEvent->eventData());
    const int entityId = iData->entityId();
    EntityMap* entityMap = m_privHash->findEntity(entityId);

    if(entityMap)
        ErrorDataSender::errorOut(QString("Cannot add entity, entity id already exists: %1").arg(iData->entityId()), event, this);
    else {
        m_privHash->insertEntity(entityId);
        QStringList components = iData->jsonData().toVariantHash().value("components").toStringList();
        for(const QString &componentName: qAsConst(components)) {
            StorageComponentPtr futureComponent = m_privHash->getFutureComponent(entityId, componentName);
            if(futureComponent)
                m_privHash->insertFutureComponent(entityId, componentName, futureComponent, QVariant());
        }
    }
}

void ClientStorageEventSystem::processComponentData(QEvent *event)
{
    CommandEvent *cEvent = static_cast<CommandEvent *>(event);
    ComponentData *cData = static_cast<ComponentData *>(cEvent->eventData());
    const QString componentName = cData->componentName();
    const int entityId = cData->entityId();
    EntityMap* entity = m_privHash->findEntity(entityId);
    StorageComponentPtr component = m_privHash->findComponent(entity, componentName);

    switch(cData->eventCommand())
    {
    case ComponentData::Command::CCMD_FETCH:
        if(!entity)
            ErrorDataSender::errorOut(QString("Cannot fetch component for not existing entity id: %1").arg(entityId), event, this);
        else if(!component)
            ErrorDataSender::errorOut(QString("Cannot fetch not existing component: %1 %2").arg(entityId).arg(cData->componentName()), event, this);
        else
            m_privHash->insertComponentValue(entity, componentName, cData->newValue());
        break;
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
        else
            m_privHash->changeComponentValue(component, cData->newValue());
        break;
    case ComponentData::Command::CCMD_ADD:
        if(!entity)
            ErrorDataSender::errorOut(QString("Cannot add component for not existing entity id: %1").arg(entityId), event, this);
        else if(component)
            ErrorDataSender::errorOut(QString("Cannot add not existing component: %1 %2").arg(entityId).arg(cData->componentName()), event, this);
        else {
            StorageComponentPtr futureComponent = m_privHash->getFutureComponent(entityId, componentName);
            if(futureComponent)
                m_privHash->insertFutureComponent(entityId, componentName, futureComponent, cData->newValue());
        }
        break;
    default:
        break;
    }
}

void ClientStorageEventSystem::processEntityData(QEvent *event)
{
    CommandEvent *cEvent = static_cast<CommandEvent *>(event);
    EntityData *eData = static_cast<EntityData *>(cEvent->eventData());
    const int entityId = eData->entityId();
    EntityMap* entityMap = m_privHash->findEntity(entityId);
    switch(eData->eventCommand())
    {
    case EntityData::Command::ECMD_REMOVE:
    {
        if(!entityMap)
            ErrorDataSender::errorOut(QString("Cannot delete entity, entity id does not exists: %1").arg(entityId), event, this);
        else
            m_privHash->removeEntity(entityId);
        break;
    }
    default:
        break;
    }

}
