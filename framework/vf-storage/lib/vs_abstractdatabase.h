#ifndef VS_ABSTRACTDATABASE_H
#define VS_ABSTRACTDATABASE_H

#include "vs_abstractcomponent.h"

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractDatabase
{
public:
    virtual ~AbstractDatabase() = default;

    virtual bool hasEntity(int entityId) const = 0;
    virtual QList<int> getEntityList() const = 0;

    virtual bool hasStoredValue(int entityId, const QString &componentName) const = 0;
    virtual bool hasFutureStoredValue(int entityId, const QString &componentName) const = 0;

    virtual QVariant getStoredValue(int entityId, const QString &componentName) const = 0;
    virtual QVariant getFutureStoredValue(int entityId, const QString &componentName) const = 0;

    virtual const AbstractComponentPtr findComponent(const int entityId, const QString &componentName) const = 0;
    struct EntityComponent {
        int entityId = -1;
        AbstractComponentPtr component;
    };
    virtual const QList<EntityComponent> findAllComponents(const QString &componentName) const = 0;
    virtual QList<QString> getComponentList(int entityId) const = 0;

    virtual bool areFutureComponentsEmpty() const = 0;
    virtual const AbstractComponentPtr getFutureComponent(int entityId, const QString &componentName) = 0;
    virtual const AbstractComponentPtr findFutureComponent(int entityId, const QString &componentName) const = 0;
    virtual QList<QString> getComponentListWithFutures(int entityId) const = 0;
};

}
#endif // VS_ABSTRACTDATABASE_H
