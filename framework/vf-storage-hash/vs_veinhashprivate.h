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
private:
    friend class VeinHashPrivate;
    void setValue(QVariant value);
    QVariant m_value;
};

typedef std::shared_ptr<StorageComponent> StorageComponentPtr;
typedef QHash<QString, StorageComponentPtr> EntityMap;

class VeinHashPrivate
{
public:
    VeinHashPrivate();
    void insertComponentValue(EntityMap* entityChecked, const QString &componentName, QVariant value);
    void changeComponentValue(StorageComponentPtr componentChecked, QVariant value);
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName);
    void insertEntity(const int entityId);
    void removeEntity(const int entityId);

    QList<int> getEntityList() const;
    QList<QString> getComponentList(int entityId);
    EntityMap *findEntity(const int entityId);
    StorageComponentPtr findComponent(EntityMap *entityMap, const QString &componentName);
    StorageComponentPtr findComponent(const int entityId, const QString &componentName);
private:
    QHash<int, EntityMap> m_entityComponentData;
};

}

#endif // VEINHASHPRIVATE_H
