#include "vs_databasehash.h"
#include "vs_storagecomponent.h"
#include <QDateTime>

namespace VeinStorage
{

bool DatabaseHash::hasEntity(int entityId) const
{
    return m_entityComponentData.constFind(entityId) != m_entityComponentData.constEnd();
}

QList<int> DatabaseHash::getEntityList() const
{
    return m_entityComponentData.keys();
}

bool DatabaseHash::hasStoredValue(int entityId, const QString &componentName) const
{
    return findComponent(entityId, componentName) != nullptr;
}

QVariant DatabaseHash::getStoredValue(int entityId, const QString &componentName) const
{
    const AbstractComponentPtr component = findComponent(entityId, componentName);
    if (component != nullptr)
        return component->getValue();
    qWarning("Stored value not found: EntityId: %i / ComponentName: %s", entityId, qPrintable(componentName));
    return QVariant();
}

const AbstractComponentPtr DatabaseHash::findComponent(const int entityId, const QString &componentName) const
{
    auto iterEntity = m_entityComponentData.constFind(entityId);
    if(iterEntity == m_entityComponentData.constEnd())
        return nullptr;
    const EntityMap &entityMap = iterEntity.value();
    return findComponent(&entityMap, componentName);
}

QList<QString> DatabaseHash::getComponentList(int entityId) const
{
    auto iter = m_entityComponentData.constFind(entityId);
    if(iter != m_entityComponentData.constEnd())
        return iter.value().keys();
    return QList<QString>();
}

bool DatabaseHash::areFutureComponentsEmpty() const
{
    return m_futureEntityComponentData.isEmpty();
}

const AbstractComponentPtr DatabaseHash::findFutureComponent(int entityId, const QString &componentName) const
{
    auto iterEntity = m_futureEntityComponentData.constFind(entityId);
    if(iterEntity == m_futureEntityComponentData.constEnd())
        return nullptr;
    const EntityMap &entityMap = iterEntity.value();
    return findComponent(&entityMap, componentName);
}

const AbstractComponentPtr DatabaseHash::getFutureComponent(int entityId, const QString &componentName)
{
    if(m_entityComponentData.contains(entityId) && m_entityComponentData[entityId].contains(componentName))
        return m_entityComponentData[entityId][componentName];

    if(m_futureEntityComponentData.contains(entityId) && m_futureEntityComponentData[entityId].contains(componentName))
        return m_futureEntityComponentData[entityId][componentName];

    AbstractComponentPtr newFuture = std::make_shared<StorageComponent>(QVariant());
    m_futureEntityComponentData[entityId][componentName] = newFuture;
    return newFuture;
}

QList<QString> DatabaseHash::getComponentListWithFutures(int entityId) const
{
    auto iter = m_futureEntityComponentData.constFind(entityId);
    QList<QString> componentsFuture;
    if(iter != m_futureEntityComponentData.constEnd())
        componentsFuture = iter.value().keys();

    return getComponentList(entityId) + componentsFuture;
}

EntityMap *DatabaseHash::findEntity(int entityId)
{
    auto iter = m_entityComponentData.find(entityId);
    if(iter == m_entityComponentData.end())
        return nullptr;
    return &(iter.value());
}

void DatabaseHash::insertEntity(int entityId)
{
    m_entityComponentData.insert(entityId, EntityMap());
}

void DatabaseHash::removeEntity(int entityId)
{
    m_entityComponentData.remove(entityId);
}

AbstractComponentPtr DatabaseHash::findComponent(const EntityMap *entityMap, const QString &componentName) const
{
    if(!entityMap)
        return nullptr;
    auto iter = entityMap->constFind(componentName);
    if(iter == entityMap->end())
        return nullptr;
    return iter.value();
}

void DatabaseHash::insertComponentValue(int entityId, const QString &componentName, const QVariant &value)
{
    AbstractComponentPtr future = takeFutureComponent(entityId, componentName);
    if(future) {
        future->setValue(value);
        m_entityComponentData[entityId][componentName] = future;
        return;
    }
    m_entityComponentData[entityId][componentName] = std::make_shared<StorageComponent>(value);
}

void DatabaseHash::removeComponentValue(EntityMap *entityChecked, const QString &componentName)
{
    entityChecked->remove(componentName);
}

AbstractComponentPtr DatabaseHash::takeFutureComponent(const int entityId, const QString &componentName)
{
    if(!m_futureEntityComponentData.contains(entityId))
        return nullptr;
    EntityMap &entities = m_futureEntityComponentData[entityId];
    if(!entities.contains(componentName))
        return nullptr;
    AbstractComponentPtr futureComponent = entities.take(componentName);
    if(entities.isEmpty())
        m_futureEntityComponentData.remove(entityId);
    return futureComponent;
}

}
