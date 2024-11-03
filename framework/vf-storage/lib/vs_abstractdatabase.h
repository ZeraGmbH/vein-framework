#ifndef VS_ABSTRACTDATABASE_H
#define VS_ABSTRACTDATABASE_H

#include "vs_storagecomponent.h"

namespace VeinStorage
{

class AbstractDatabase
{
public:
    virtual ~AbstractDatabase() = default;

    virtual StorageComponentPtr getFutureComponent(int entityId, const QString &componentName) = 0;
    virtual void insertComponentValue(EntityMap* entityChecked, const QString &componentName, QVariant value) = 0;
    virtual void insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, QVariant value) = 0;
    virtual void changeComponentValue(StorageComponentPtr componentChecked, QVariant value) = 0;
    virtual void removeComponentValue(EntityMap* entityChecked, const QString &componentName) = 0;
    virtual void insertEntity(const int entityId) = 0;
    virtual void removeEntity(const int entityId) = 0;

    virtual QList<int> getEntityList() const = 0;
    virtual QList<QString> getComponentList(int entityId) = 0;
    virtual EntityMap *findEntity(const int entityId) = 0;
    virtual StorageComponentPtr findComponent(EntityMap *entityMap, const QString &componentName) = 0;
    virtual StorageComponentPtr findComponent(const int entityId, const QString &componentName) = 0;
    virtual StorageComponentPtr takeFutureComponent(const int entityId, const QString &componentName) = 0;
    virtual bool areFutureComponentsEmpty() = 0;
};

}
#endif // VS_ABSTRACTDATABASE_H
