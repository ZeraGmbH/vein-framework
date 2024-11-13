#include "entitydictionary.h"

namespace VeinApiQml {

EntityDictionary::~EntityDictionary()
{
    for(auto &entry : qAsConst(m_entitiesById))
        entry.m_entityComponentMap->deleteLater();
    m_entitiesById.clear();
}

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
    if(name.isEmpty()) {
        qWarning("Cannot set empty entity name - id: %i!", entityId);
        return false;
    }
    auto iter = m_entitiesById.find(entityId);
    if(iter != m_entitiesById.end()) {
        DictEntry &entry = iter.value();
        if(entry.m_entityName == name)
            return true;
        if(m_entitiesByName.contains(name)) {
            qWarning("Entity name '%s' is not unique!", qPrintable(name));
            return false;
        }
        if(!entry.m_entityName.isEmpty() && entry.m_entityName != name) {
            qWarning("Renaming entity %i from '%s' to '%s' is not supported!",
                     entityId, qPrintable(entry.m_entityName), qPrintable(name));
            return false;
        }
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
