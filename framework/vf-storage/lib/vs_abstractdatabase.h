#ifndef VS_ABSTRACTDATABASE_H
#define VS_ABSTRACTDATABASE_H

#include "vs_storagecomponent.h"

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractDatabase
{
public:
    virtual ~AbstractDatabase() = default;

    virtual bool hasEntity(int entityId) const = 0;
    virtual EntityMap *findEntity(const int entityId) = 0;
    virtual QList<int> getEntityList() const = 0;

    virtual bool hasStoredValue(int entityId, const QString &componentName) const = 0;
    virtual QVariant getStoredValue(int t_entityId, const QString &t_componentName) const = 0;
    virtual StorageComponentPtr findComponent(const int entityId, const QString &componentName) = 0;
    virtual QList<QString> getComponentList(int entityId) const = 0;

    virtual bool areFutureComponentsEmpty() const = 0;
    virtual StorageComponentPtr getFutureComponent(int entityId, const QString &componentName) = 0;
};

}
#endif // VS_ABSTRACTDATABASE_H
