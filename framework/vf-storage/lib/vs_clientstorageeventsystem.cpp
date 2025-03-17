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
        if(m_acceptableOrigins.contains(evData->eventOrigin())) {
            if(cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION) {
                switch (evData->type())
                {
                case IntrospectionData::dataType():
                    processIntrospectionData(event);
                    break;
                case ComponentData::dataType():
                    processComponentData(event);
                    break;
                case EntityData::dataType():
                {
                    //ECMD_ADD event doesn't reach to client because client hasn't yet subscribed to that entity
                    //ECMD_SUBSCRIBE, ECMD_UNSUBSCRIBE, events do not reach to client side
                    EntityData *eData = static_cast<EntityData*>(cEvent->eventData());
                    if(eData->eventCommand() == EntityData::Command::ECMD_REMOVE)
                        m_privHash->removeEntity(eData->entityId());
                    break;
                }
                default:
                    break;
                }
            }
            // From vf-qml sitting above us (not sent out attow)
            else if(cEvent->eventSubtype() == CommandEvent::EventSubtype::TRANSACTION) {
                switch (evData->type())
                {
                case EntityData::dataType(): {
                    EntityData *eData = static_cast<EntityData*>(cEvent->eventData());
                    if(eData->eventCommand() == EntityData::Command::ECMD_UNSUBSCRIBE)
                        m_privHash->removeEntity(eData->entityId());
                    break;
                    }
                default:
                    break;
                }
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

    if(!entityMap) {
        m_privHash->insertEntity(entityId);
        entityMap = m_privHash->findEntity(entityId);
        QStringList components = iData->jsonData().toVariantHash().value("components").toStringList();
        for(const QString &componentName: qAsConst(components)) {
            StorageComponentPtr component = m_privHash->findComponent(entityId, componentName);
            if(!component)
                m_privHash->insertComponentValue(entityMap, componentName, QVariant());
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
            ErrorDataSender::errorOut(QString("Cannot add existing component: %1 %2").arg(entityId).arg(cData->componentName()), event, this);
        else
            m_privHash->insertComponentValue(entity, componentName, cData->newValue());
        break;
    default:
        break;
    }
}
