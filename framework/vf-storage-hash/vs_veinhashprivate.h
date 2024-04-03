#ifndef VEINHASHPRIVATE_H
#define VEINHASHPRIVATE_H

#include "ve_storagesystem.h"
#include <QObject>
#include <QHash>

namespace VeinStorage
{

class StorageComponent
{
public:
    StorageComponent(QVariant value);
    virtual ~StorageComponent();
    QVariant getValue() const;
    void setValue(QVariant value);
    VeinEvent::ComponentChangeSignalizer* getChangeSignalizer();
private:
    QVariant m_value;
    VeinEvent::ComponentChangeSignalizer *m_changeSignalizer = nullptr; // maybe a list??
};

typedef QHash<QString, StorageComponent> EntityMap;

class VeinHashPrivate
{
public:
    VeinHashPrivate();
    void insertComponentValue(EntityMap* entityChecked, const QString &componentName, QVariant value);
    void changeComponentValue(StorageComponent* componentChecked, QVariant value);
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName);
    void insertEntity(const int entityId);
    void removeEntity(const int entityId);

    QList<int> getEntityList() const;
    QList<QString> getComponentList(int entityId);
    EntityMap *findEntity(const int entityId);
    StorageComponent* findComponent(EntityMap *entityMap, const QString &componentName);
    StorageComponent* findComponent(const int entityId, const QString &componentName);
private:
    QHash<int, EntityMap> m_entityComponentData;
};

}

#endif // VEINHASHPRIVATE_H
