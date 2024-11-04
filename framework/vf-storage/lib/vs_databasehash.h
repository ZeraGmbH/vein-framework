#ifndef VS_DATABASE_HASH
#define VS_DATABASE_HASH

#include "vs_abstractdatabase.h"
#include <QObject>
#include <QHash>
#include <QDateTime>

namespace VeinStorage
{

class DatabaseHash : public AbstractDatabase
{
public:
    // Outer interface
    bool hasEntity(int entityId) const override;
    QList<int> getEntityList() const override;

    bool hasStoredValue(int entityId, const QString &componentName) const override;
    QVariant getStoredValue(int entityId, const QString &componentName) const override;
    StorageComponentPtr findComponent(const int entityId, const QString &componentName) override;
    QList<QString> getComponentList(int entityId) const override;

    bool areFutureComponentsEmpty() const override;
    StorageComponentPtr getFutureComponent(int entityId, const QString &componentName) override;

    // Event system interface
    // ATOW event systems use conrete DatabaseHash. Once we add another implementation
    // of DatabaseHash, we have to think about an interface as AbstractDatabaseWritable
    EntityMap *findEntity(const int entityId);
    void insertEntity(const int entityId);
    void removeEntity(const int entityId);

    StorageComponentPtr findComponent(EntityMap *entityMap, const QString &componentName);
    void insertComponentValue(EntityMap* entityChecked, const QString &componentName, QVariant value);
    void changeComponentValue(StorageComponentPtr componentChecked, QVariant value);
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName);

    void insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, QVariant value);
    StorageComponentPtr takeFutureComponent(const int entityId, const QString &componentName);

private:
    QHash<int, EntityMap> m_entityComponentData;
    QHash<int, EntityMap> m_futureEntityComponentData;
};

}

#endif // VS_DATABASE_HASH
