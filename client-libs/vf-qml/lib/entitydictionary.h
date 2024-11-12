#ifndef ENTITYDICTIONARY_H
#define ENTITYDICTIONARY_H

#include "entitycomponentmap.h"
#include <QHash>

namespace VeinApiQml {

class EntityDictionary
{
public:
    bool insert(int entityId, EntityComponentMap* eMap);
    bool setEntityName(int entityId, const QString& name);

    QList<int> getEntityList() const;
    QString nameFromId(int entityId) const;
    EntityComponentMap* findByName(const QString& entityName) const;
    EntityComponentMap* findById(int entityId) const;
    EntityComponentMap* remove(int entityId);
private:
    struct DictEntry
    {
        QString m_entityName;
        EntityComponentMap* m_entityComponentMap = nullptr;
    };
    QHash<int, DictEntry> m_entitiesById;
    QHash<QString, EntityComponentMap*> m_entitiesByName;
};

}
#endif // ENTITYDICTIONARY_H
