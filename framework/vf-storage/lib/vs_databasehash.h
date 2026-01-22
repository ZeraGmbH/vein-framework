#ifndef VS_DATABASE_HASH
#define VS_DATABASE_HASH

#include "vs_abstractdatabasedirectwrite.h"
#include <QObject>
#include <QHash>
#include <QDateTime>

namespace VeinStorage
{

class DatabaseHash : public AbstractDatabaseDirectWrite
{
public:
    // Outer interface to vein/enttity components / future components are ignored
    bool hasEntity(int entityId) const override;
    QList<int> getEntityList() const override;

    bool hasStoredValue(int entityId, const QString &componentName) const override;
    QVariant getStoredValue(int entityId, const QString &componentName) const override;
    const StorageComponentPtr findComponent(const int entityId, const QString &componentName) const override;
    QList<QString> getComponentList(int entityId) const override;

    // Outer interface to future components
    // 'future' means maybe added by vein later (or maybe never)
    // Futures are ignored by introspection but can be fetched
    bool areFutureComponentsEmpty() const override;
    const StorageComponentPtr findFutureComponent(int entityId, const QString &componentName) const override;
    const StorageComponentPtr getFutureComponent(int entityId, const QString &componentName) override;
    StorageComponentPtr getFutureComponentForWrite(int entityId, const QString &componentName) override;
    QList<QString> getComponentListWithFutures(int entityId) const override;

    // Event system (vein) interface
    EntityMap *findEntity(int entityId);
    void insertEntity(int entityId);
    void removeEntity(int entityId);

    StorageComponentPtr findComponent(const EntityMap *entityMap, const QString &componentName) const;
    void insertComponentValue(EntityMap* entityChecked, const QString &componentName, const QVariant &value);
    void changeComponentValue(StorageComponentPtr componentChecked, const QVariant &value);
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName);

    void insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, const QVariant &value);
    StorageComponentPtr takeFutureComponent(const int entityId, const QString &componentName);

private:
    QMap<int, EntityMap> m_entityComponentData;
    QMap<int, EntityMap> m_futureEntityComponentData;
};

}

#endif // VS_DATABASE_HASH
