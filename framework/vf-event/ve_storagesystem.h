#ifndef STORAGESYSTEM_H
#define STORAGESYSTEM_H

#include "vfevent_export.h"
#include "ve_eventsystem.h"
#include <QIODevice>

Q_DECLARE_LOGGING_CATEGORY(VEIN_STORAGE)

namespace VeinEvent
{
class VFEVENT_EXPORT StorageSystem : public EventSystem
{
    Q_OBJECT
public:
    explicit StorageSystem(QObject *t_parent=nullptr);

    virtual QVariant getStoredValue(int t_entityId, const QString &t_componentName) const = 0;
    virtual bool hasStoredValue(int t_entityId, const QString &t_componentName) const = 0;

    virtual QList<QString> getEntityComponents(int t_entityId) const = 0;
    virtual bool hasEntity(int t_entityId) const = 0;
    virtual QList<int> getEntityList() const = 0;

    virtual void dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter) const = 0;
};
}

#endif // STORAGESYSTEM_H

