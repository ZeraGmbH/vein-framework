#ifndef VS_STORAGEFILTER_H
#define VS_STORAGEFILTER_H

#include "vs_abstracteventsystem.h"
#include "vs_abstracttimestamper.h"
#include "vs_timestampernow.h"
#include <QList>
#include <QHash>

namespace VeinStorage
{

class StorageFilter : public QObject
{
    Q_OBJECT
public:
    struct Settings {
        Settings(bool fireCurrentValueOnAddFilter, bool fireOnChangesOnly);
        bool m_fireCurrentValueOnAddFilter;
        bool m_fireOnChangesOnly;
    };
    explicit StorageFilter(AbstractEventSystem* storage, Settings settings);
    ~StorageFilter();
    bool add(int entityId, QString componentName, AbstractTimeStamperPtr timeStamper = TimeStamperNow::getInstance());
    void clear();
signals:
    void sigComponentValue(int entityId, QString componentName, QVariant value, QDateTime timestamp);

private:
    void fireActual(int entityId, QString componentName, AbstractComponentPtr actualComponent, QDateTime timestamp);
    const AbstractEventSystem* m_storage;
    const Settings m_settings;
    QList<QMetaObject::Connection> m_componentChangeConnections;
    QHash<int, QSet<QString>> m_filteredEntityComponents;
};

}
#endif // VS_STORAGEFILTER_H
