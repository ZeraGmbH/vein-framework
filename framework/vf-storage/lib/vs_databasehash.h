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
    bool hasEntity(int entityId) const override;
    EntityMap *findEntity(const int entityId) override;
    QList<int> getEntityList() const override;
    void insertEntity(const int entityId) override;
    void removeEntity(const int entityId) override;

    bool hasStoredValue(int entityId, const QString &componentName) const override;
    QVariant getStoredValue(int entityId, const QString &componentName) const override;
    StorageComponentPtr findComponent(EntityMap *entityMap, const QString &componentName) override;
    StorageComponentPtr findComponent(const int entityId, const QString &componentName) override;
    QList<QString> getComponentList(int entityId) override;
    void insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, QVariant value) override;
    void changeComponentValue(StorageComponentPtr componentChecked, QVariant value) override;
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName) override;

    bool areFutureComponentsEmpty() const override;
    StorageComponentPtr getFutureComponent(int entityId, const QString &componentName) override;
    void insertComponentValue(EntityMap* entityChecked, const QString &componentName, QVariant value) override;
    StorageComponentPtr takeFutureComponent(const int entityId, const QString &componentName) override;

private:
    QHash<int, EntityMap> m_entityComponentData;
    QHash<int, EntityMap> m_futureEntityComponentData;
};

}

#endif // VS_DATABASE_HASH
