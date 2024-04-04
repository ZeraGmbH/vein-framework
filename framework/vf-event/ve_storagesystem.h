#ifndef STORAGESYSTEM_H
#define STORAGESYSTEM_H

#include "vfevent_export.h"
#include "ve_eventsystem.h"
#include <QIODevice>

namespace VeinEvent
{

class VFEVENT_EXPORT ComponentChangeSignalizer : public QObject
{
    Q_OBJECT
signals:
    void sigValueChange(QVariant newValue);
};

class VFEVENT_EXPORT StorageSystem : public EventSystem
{
    Q_OBJECT
public:
    explicit StorageSystem(QObject *parent = nullptr);

    virtual bool hasEntity(int t_entityId) const = 0;
    virtual QList<int> getEntityList() const = 0;

    virtual bool hasStoredValue(int t_entityId, const QString &t_componentName) const = 0;
    virtual QVariant getStoredValue(int t_entityId, const QString &t_componentName) const = 0;
    virtual QList<QString> getEntityComponents(int t_entityId) const = 0;
    virtual VeinEvent::ComponentChangeSignalizer* getChangeSignalizer(int entityId, const QString &componentName) const = 0;

    virtual void dumpToFile(QIODevice *outputFileDevice, QList<int> entityFilter) const = 0;
};
}

#endif // STORAGESYSTEM_H

