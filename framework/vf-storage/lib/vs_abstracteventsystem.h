#ifndef VS_ABSTRACTSTORAGESYSTEM_H
#define VS_ABSTRACTSTORAGESYSTEM_H

#include "vfevent_export.h"
#include "vs_abstractdatabase.h"
#include "ve_eventsystem.h"

namespace VeinStorage
{

class VFEVENT_EXPORT AbstractEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit AbstractEventSystem(QObject *parent = nullptr) : EventSystem(parent) {}

    virtual AbstractDatabase* getDb() const = 0;

    virtual bool hasEntity(int t_entityId) const = 0;

    virtual bool hasStoredValue(int t_entityId, const QString &t_componentName) const = 0;
    virtual QVariant getStoredValue(int t_entityId, const QString &t_componentName) const = 0;
};
}

#endif // VS_ABSTRACTSTORAGESYSTEM_H

