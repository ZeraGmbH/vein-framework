#ifndef VEINHASHPRIVATE_H
#define VEINHASHPRIVATE_H

#include "ve_storagesystem.h"
#include <QObject>
#include <QHash>

namespace VeinStorage
{

class StorageComponent : public VeinEvent::StorageComponentInterface
{
    Q_OBJECT
public:
    StorageComponent(QVariant value);
    QVariant getValue() const override;
    QString getTimestamp() const override;
private:
    friend class VeinHashPrivate;
    void setValue(QVariant value);
    void setTimestamp(QString timestamp);
    QVariant m_value;
    QString m_timestamp;
};

typedef std::shared_ptr<StorageComponent> StorageComponentPtr;
typedef QHash<QString, StorageComponentPtr> EntityMap;

class VeinHashPrivate
{
public:
    VeinHashPrivate();
    StorageComponentPtr getFutureComponent(int entityId, const QString &componentName);
    void insertComponentValue(EntityMap* entityChecked, const QString &componentName, QVariant value);
    void insertFutureComponent(int entityId, QString componentName, StorageComponentPtr component, QVariant value);
    void changeComponentValue(StorageComponentPtr componentChecked, QVariant value, QString timestamp);
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName);
    void insertEntity(const int entityId);
    void removeEntity(const int entityId);

    QList<int> getEntityList() const;
    QList<QString> getComponentList(int entityId);
    EntityMap *findEntity(const int entityId);
    StorageComponentPtr findComponent(EntityMap *entityMap, const QString &componentName);
    StorageComponentPtr findComponent(const int entityId, const QString &componentName);
    StorageComponentPtr takeFutureComponent(const int entityId, const QString &componentName);
    bool areFutureComponentsEmpty();
private:
    QHash<int, EntityMap> m_entityComponentData;
    QHash<int, EntityMap> m_futureEntityComponentData;
};

}

#endif // VEINHASHPRIVATE_H
