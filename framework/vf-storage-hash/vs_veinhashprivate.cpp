#include <QDateTime>
#include <timerfactoryqt.h>
#include "vs_veinhashprivate.h"

using namespace VeinEvent;

namespace VeinStorage
{

StorageComponent::StorageComponent(QVariant value) :
    m_value(value)
{
}

QVariant StorageComponent::getValue() const
{
    return m_value;
}

QDateTime StorageComponent::getTimestamp() const
{
    return m_timestamp;
}

void StorageComponent::setValue(QVariant value)
{
    emit sigValueSet(value);
    if(value != m_value) {
        m_value = value;
        emit sigValueChange(value);
    }
}

void StorageComponent::setTimestamp(QDateTime timestamp)
{
    m_timestamp = timestamp;
}

VeinHashPrivate::VeinHashPrivate()
{
}

StorageComponentPtr VeinHashPrivate::getFutureComponent(int entityId, const QString &componentName)
{
    if(m_entityComponentData.contains(entityId) && m_entityComponentData[entityId].contains(componentName))
        return m_entityComponentData[entityId][componentName];

    if(m_futureEntityComponentData.contains(entityId) && m_futureEntityComponentData[entityId].contains(componentName))
        return m_futureEntityComponentData[entityId][componentName];

    StorageComponentPtr newFuture = std::make_shared<StorageComponent>(QVariant());
    m_futureEntityComponentData[entityId][componentName] = newFuture;
    return newFuture;
}

void VeinHashPrivate::insertComponentValue(EntityMap *entityChecked, const QString &componentName, QVariant value)
{
    (*entityChecked)[componentName] = std::make_shared<StorageComponent>(value);
    StorageComponentPtr component = (*entityChecked)[componentName];
    setComponentTimeStamp(component);
}

void VeinHashPrivate::insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, QVariant value)
{
    Q_ASSERT(!m_entityComponentData.contains(entityId) || !m_entityComponentData[entityId].contains(componentName));
    setComponentTimeStamp(component);
    component->setValue(value);
    m_entityComponentData[entityId][componentName] = component;
}

void VeinHashPrivate::changeComponentValue(StorageComponentPtr componentChecked, QVariant value)
{
    setComponentTimeStamp(componentChecked);
    componentChecked->setValue(value);
}

void VeinHashPrivate::removeComponentValue(EntityMap *entityChecked, const QString &componentName)
{
    entityChecked->remove(componentName);
}

void VeinHashPrivate::insertEntity(const int entityId)
{
    m_entityComponentData.insert(entityId, EntityMap());
}

void VeinHashPrivate::removeEntity(const int entityId)
{
    m_entityComponentData.remove(entityId);
}

QList<int> VeinHashPrivate::getEntityList() const
{
    return m_entityComponentData.keys();
}

QList<QString> VeinHashPrivate::getComponentList(int entityId)
{
    QList<QString> componentList;
    EntityMap *entity = findEntity(entityId);
    if(entity)
        componentList = entity->keys();
    return componentList;
}

EntityMap *VeinHashPrivate::findEntity(const int entityId)
{
    auto iter = m_entityComponentData.find(entityId);
    if(iter == m_entityComponentData.end())
        return nullptr;
    return &(iter.value());
}

StorageComponentPtr VeinHashPrivate::findComponent(EntityMap *entityMap, const QString &componentName)
{
    if(!entityMap)
        return nullptr;
    auto iter = entityMap->find(componentName);
    if(iter == entityMap->end())
        return nullptr;
    return iter.value();
}

StorageComponentPtr VeinHashPrivate::findComponent(const int entityId, const QString &componentName)
{
    return findComponent(findEntity(entityId), componentName);
}

StorageComponentPtr VeinHashPrivate::takeFutureComponent(const int entityId, const QString &componentName)
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

bool VeinHashPrivate::areFutureComponentsEmpty()
{
    return m_futureEntityComponentData.isEmpty();
}

void VeinHashPrivate::setComponentTimeStamp(StorageComponentPtr component)
{
    QDateTime now = TimerFactoryQt::getCurrentTime();
    component->setTimestamp(now);
}

}
