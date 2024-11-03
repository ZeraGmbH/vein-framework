#ifndef VS_DATABASE_HASH
#define VS_DATABASE_HASH

#include "vs_storagecomponent.h"
#include <QObject>
#include <QHash>
#include <QDateTime>

namespace VeinStorage
{

class DatabaseHash
{
public:
    DatabaseHash();
    StorageComponentPtr getFutureComponent(int entityId, const QString &componentName);
    void insertComponentValue(EntityMap* entityChecked, const QString &componentName, QVariant value);
    void insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, QVariant value);
    void changeComponentValue(StorageComponentPtr componentChecked, QVariant value);
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName);
    void insertEntity(const int entityId);
    void removeEntity(const int entityId);

    QList<int> getEntityList() const;
    QList<QString> getComponentList(int entityId);
    EntityMap *findEntity(const int entityId);
    StorageComponentPtr findComponent(EntityMap *entityMap, const QString &componentName);
    StorageComponentPtr findComponent(const int entityId, const QString &componentName);
    StorageComponentPtr takeFutureComponent(const int entityId, const QString &componentName);
    bool areFutureComponentsEmpty();
private:
    QHash<int, EntityMap> m_entityComponentData;
    QHash<int, EntityMap> m_futureEntityComponentData;
};

}

#endif // VS_DATABASE_HASH
