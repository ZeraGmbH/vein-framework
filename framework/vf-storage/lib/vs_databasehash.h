#ifndef VS_DATABASE_HASH
#define VS_DATABASE_HASH

#include "vs_abstractdatabase.h"
#include <QObject>
#include <QHash>
#include <QDateTime>

namespace VeinStorage
{

class DatabaseHash : public AbstractDatabase
{
public:
    // Consumer interface to vein / entity components available at the time of call
    bool hasEntity(int entityId) const override;
    QList<int> getEntityList() const override;

    bool hasStoredValue(int entityId, const QString &componentName) const override;
    QVariant getStoredValue(int entityId, const QString &componentName) const override;
    const AbstractComponentPtr findComponent(const int entityId, const QString &componentName) const override;
    QList<QString> getComponentList(int entityId) const override;

    // Consumer interface to future components - where 'future' means:
    // * getFutureComponent(ForWrite) always return a storage component whether or not the component
    //   is known to vein at the time of call => modules can access components of modules that are started later
    // * In case getFutureComponent(ForWrite) generates components that are never added to vein they remain hidden
    //   to introspection and will not send out any notifications with one exception: Future-only components
    //   respond to CCMD_FETCH so data is made available on demand. Note: future components with entity IDs not
    //   known to vein are competely ignored
    // * To make them discoverable getComponentListWithFutures was added
    // TODO: reset future components on session change / handle CCMD_REMOVE?
    bool areFutureComponentsEmpty() const override;
    const AbstractComponentPtr findFutureComponent(int entityId, const QString &componentName) const override;
    const AbstractComponentPtr getFutureComponent(int entityId, const QString &componentName) override;
    QList<QString> getComponentListWithFutures(int entityId) const override;

    // Event system (vein) interface
    EntityMap *findEntity(int entityId);
    void insertEntity(int entityId);
    void removeEntity(int entityId);

    AbstractComponentPtr findComponent(const EntityMap *entityMap, const QString &componentName) const;
    void insertComponentValue(int entityId, const QString &componentName, const QVariant &value);
    void removeComponentValue(EntityMap* entityChecked, const QString &componentName);

private:
    AbstractComponentPtr takeFutureComponent(const int entityId, const QString &componentName);

    QMap<int, EntityMap> m_entityComponentData;
    QMap<int, EntityMap> m_futureEntityComponentData;
};

}

#endif // VS_DATABASE_HASH
