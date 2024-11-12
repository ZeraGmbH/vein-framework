#include "entitydictionary.h"

namespace VeinApiQml {

bool VeinApiQml::EntityDictionary::insert(int entityId, EntityComponentMap* eMap)
{
    if(!m_entitiesById.contains(entityId)) {
        DictEntry entry;
        entry.m_entityComponentMap = eMap;
        m_entitiesById[entityId] = entry;
        return true;
    }
    return false;
}

bool EntityDictionary::setEntityName(int entityId, const QString &name)
{
    auto iter = m_entitiesById.find(entityId);
    if(iter != m_entitiesById.end() && !m_entitiesByName.contains(name)) {
        DictEntry &entry = iter.value();
        if(!entry.m_entityName.isEmpty())
            return false;
        entry.m_entityName = name;
        m_entitiesByName[name] = entry.m_entityComponentMap;
        return true;
    }
    return false;
}

QList<int> EntityDictionary::getEntityList() const
{
    return m_entitiesById.keys();
}

QString EntityDictionary::nameFromId(int entityId) const
{
    QString name;
    auto iter = m_entitiesById.constFind(entityId);
    if(iter != m_entitiesById.constEnd())
        name = iter.value().m_entityName;
    return name;
}

EntityComponentMap *EntityDictionary::findByName(const QString &entityName) const
{
    auto iter = m_entitiesByName.constFind(entityName);
    if(iter != m_entitiesByName.constEnd())
        return iter.value();
    return nullptr;
}

EntityComponentMap *EntityDictionary::findById(int entityId) const
{
    auto iter = m_entitiesById.constFind(entityId);
    if(iter != m_entitiesById.constEnd())
        return iter.value().m_entityComponentMap;
    return nullptr;
}

EntityComponentMap *EntityDictionary::remove(int entityId)
{
    DictEntry entry = m_entitiesById.take(entityId);
    m_entitiesByName.remove(entry.m_entityName);
    return entry.m_entityComponentMap;
}

}
