#include "vs_databasehash.h"
#include <QDateTime>
#include <QDebug>

namespace VeinStorage
{

const StorageComponentPtr DatabaseHash::getFutureComponent(int entityId, const QString &componentName)
{
    if(m_entityComponentData.contains(entityId) && m_entityComponentData[entityId].contains(componentName))
        return m_entityComponentData[entityId][componentName];

    if(m_futureEntityComponentData.contains(entityId) && m_futureEntityComponentData[entityId].contains(componentName))
        return m_futureEntityComponentData[entityId][componentName];

    StorageComponentPtr newFuture = std::make_shared<StorageComponent>(QVariant());
    m_futureEntityComponentData[entityId][componentName] = newFuture;
    return newFuture;
}

void DatabaseHash::insertComponentValue(EntityMap *entityChecked, const QString &componentName, QVariant value)
{
    (*entityChecked)[componentName] = std::make_shared<StorageComponent>(value);
    StorageComponentPtr component = (*entityChecked)[componentName];
}

void DatabaseHash::insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, QVariant value)
{
    Q_ASSERT(!m_entityComponentData.contains(entityId) || !m_entityComponentData[entityId].contains(componentName));
    component->setValue(value);
    m_entityComponentData[entityId][componentName] = component;
}

void DatabaseHash::changeComponentValue(StorageComponentPtr componentChecked, QVariant value)
{
    componentChecked->setValue(value);
}

void DatabaseHash::removeComponentValue(EntityMap *entityChecked, const QString &componentName)
{
    entityChecked->remove(componentName);
}

void DatabaseHash::insertEntity(const int entityId)
{
    m_entityComponentData.insert(entityId, EntityMap());
}

void DatabaseHash::removeEntity(const int entityId)
{
    m_entityComponentData.remove(entityId);
}

bool DatabaseHash::hasStoredValue(int entityId, const QString &componentName) const
{
    auto entityIter = m_entityComponentData.constFind(entityId);
    if(m_entityComponentData.constFind(entityId) == m_entityComponentData.constEnd())
        return false;
    auto components = entityIter.value();
    auto componentIter = components.constFind(componentName);
    return componentIter != components.constEnd();
}

QVariant DatabaseHash::getStoredValue(int entityId, const QString &componentName) const
{
    auto entityIter = m_entityComponentData.constFind(entityId);
    if(m_entityComponentData.constFind(entityId) == m_entityComponentData.constEnd()) {
        qWarning() << "Unknown entity with id:" <<  entityId;
        return QVariant();
    }
    auto components = entityIter.value();
    auto componentIter = components.constFind(componentName);
    if(componentIter == components.constEnd()) {
        qWarning() << "Unknown entity with id:" <<  entityId << "component" << componentName;
        return QVariant();
    }
    return componentIter.value()->m_value;
}

QList<int> DatabaseHash::getEntityList() const
{
    return m_entityComponentData.keys();
}

QList<QString> DatabaseHash::getComponentList(int entityId) const
{
    auto iter = m_entityComponentData.constFind(entityId);
    QList<QString> componentList;
    if(iter != m_entityComponentData.constEnd())
        componentList = iter.value().keys();
    return componentList;
}

bool DatabaseHash::hasEntity(int entityId) const
{
    return m_entityComponentData.constFind(entityId) != m_entityComponentData.constEnd();
}

EntityMap *DatabaseHash::findEntity(const int entityId)
{
    auto iter = m_entityComponentData.find(entityId);
    if(iter == m_entityComponentData.end())
        return nullptr;
    return &(iter.value());
}

StorageComponentPtr DatabaseHash::findComponent(EntityMap *entityMap, const QString &componentName)
{
    if(!entityMap)
        return nullptr;
    auto iter = entityMap->find(componentName);
    if(iter == entityMap->end())
        return nullptr;
    return iter.value();
}

const StorageComponentPtr DatabaseHash::findComponent(const int entityId, const QString &componentName) const
{
    auto iterEntity = m_entityComponentData.constFind(entityId);
    if(iterEntity == m_entityComponentData.constEnd())
        return nullptr;
    const auto entityMap = iterEntity.value();
    auto iterComponent = entityMap.constFind(componentName);
    if(iterComponent == entityMap.constEnd())
        return nullptr;
    return iterComponent.value();
}

StorageComponentPtr DatabaseHash::takeFutureComponent(const int entityId, const QString &componentName)
{
    if(!m_futureEntityComponentData.contains(entityId))
        return nullptr;
    EntityMap &entities = m_futureEntityComponentData[entityId];
    if(!entities.contains(componentName))
        return nullptr;
    StorageComponentPtr futureComponent = entities.take(componentName);
    if(entities.isEmpty())
        m_futureEntityComponentData.remove(entityId);
    return futureComponent;
}

bool DatabaseHash::areFutureComponentsEmpty() const
{
    return m_futureEntityComponentData.isEmpty();
}

}
