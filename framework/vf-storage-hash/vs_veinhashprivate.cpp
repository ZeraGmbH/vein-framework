#include "vs_veinhashprivate.h"

using namespace VeinEvent;

namespace VeinStorage
{

StorageComponent::StorageComponent()
{
}

StorageComponent::StorageComponent(QVariant value) :
    m_value(value)
{
}

StorageComponent &StorageComponent::operator =(const StorageComponent &other)
{
    m_value = other.m_value;
    return *this;
}

StorageComponent::StorageComponent(StorageComponent &other)
{
    m_value = other.m_value;
}

QVariant StorageComponent::getValue() const
{
    return m_value;
}

void StorageComponent::setValue(QVariant value)
{
    if(value != m_value) {
        m_value = value;
        emit sigValueChange(value);
    }
}

VeinHashPrivate::VeinHashPrivate()
{
}

void VeinHashPrivate::insertComponentValue(EntityMap *entityChecked, const QString &componentName, QVariant value)
{
    (*entityChecked)[componentName] = std::make_shared<StorageComponent>(value);
}

void VeinHashPrivate::changeComponentValue(StorageComponentPtr componentChecked, QVariant value)
{
    componentChecked->setValue(value);
}

void VeinHashPrivate::removeComponentValue(EntityMap *entityChecked, const QString &componentName)
{
    entityChecked->erase(componentName);
}

void VeinHashPrivate::insertEntity(const int entityId)
{
    m_entityComponentData.insert(std::pair(entityId, EntityMap()));
}

void VeinHashPrivate::removeEntity(const int entityId)
{
    m_entityComponentData.erase(entityId);
}

QList<int> VeinHashPrivate::getEntityList() const
{
    QList<int> entities;
    for(auto iter=m_entityComponentData.cbegin(); iter!=m_entityComponentData.cend(); iter++)
        entities.append(iter->first);
    return entities;
}

QList<QString> VeinHashPrivate::getComponentList(int entityId)
{
    QList<QString> componentList;
    EntityMap *entity = findEntity(entityId);
    if(entity) {
        for(auto iter=entity->cbegin(); iter!=entity->cend(); iter++)
            componentList.append(iter->first);
    }
    return componentList;
}

EntityMap *VeinHashPrivate::findEntity(const int entityId)
{
    auto iter = m_entityComponentData.find(entityId);
    if(iter == m_entityComponentData.end())
        return nullptr;
    return &(iter->second);
}

StorageComponentPtr VeinHashPrivate::findComponent(EntityMap *entityMap, const QString &componentName)
{
    if(!entityMap)
        return nullptr;
    auto iter = entityMap->find(componentName);
    if(iter == entityMap->end())
        return nullptr;
    return iter->second;
}

StorageComponentPtr VeinHashPrivate::findComponent(const int entityId, const QString &componentName)
{
    return findComponent(findEntity(entityId), componentName);
}

}
