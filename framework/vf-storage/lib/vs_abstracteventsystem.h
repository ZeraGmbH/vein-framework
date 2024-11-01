#ifndef VS_ABSTRACTSTORAGESYSTEM_H
#define VS_ABSTRACTSTORAGESYSTEM_H

#include "vfevent_export.h"
#include "vs_abstractcomponent.h"
#include "ve_eventsystem.h"
#include <QIODevice>

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit AbstractEventSystem(QObject *parent = nullptr) : EventSystem(parent) {}

    virtual bool hasEntity(int t_entityId) const = 0;
    virtual QList<int> getEntityList() const = 0;

    virtual bool hasStoredValue(int t_entityId, const QString &t_componentName) const = 0;
    virtual QVariant getStoredValue(int t_entityId, const QString &t_componentName) const = 0;
    virtual QList<QString> getEntityComponents(int t_entityId) const = 0;

    virtual AbstractComponentPtr getComponent(int entityId, const QString &componentName) const = 0;
    virtual AbstractComponentPtr getFutureComponent(int entityId, const QString &componentName) = 0;

    virtual void dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter) const = 0;
    virtual bool areFutureComponentsEmpty() = 0;
};
}

#endif // VS_ABSTRACTSTORAGESYSTEM_H

