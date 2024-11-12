#ifndef VEINQML_H
#define VEINQML_H

#include "globalIncludes.h"
#include <ve_eventsystem.h>
#include <QHash>
#include <QJsonObject>

namespace VeinApiQml
{
class EntityComponentMap;

class VFQML_EXPORT VeinQml : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit VeinQml(QObject *t_parent = nullptr);
    ~VeinQml();
    void processEvent(QEvent *t_event) override;

    enum class ConnectionState : int {
        VQ_IDLE = 0, /**< the system has been created and is not yet ready */
        VQ_LOADED = 1, /**< the required entities are set and their introspection data has been fetched */
        VQ_DISCONNECTED = 2, /**< the host or the client has closed the connection - NOT IMPLEMENTED!!!*/
        VQ_ERROR = 3 /**< error state, e.g. a required entity is not available on the server */
    };
    Q_ENUM(ConnectionState)

    Q_PROPERTY(ConnectionState state READ state NOTIFY sigStateChanged)
    ConnectionState state() const;

    Q_INVOKABLE EntityComponentMap *getEntity(const QString &t_entityName) const;
    Q_INVOKABLE bool hasEntity(const QString &t_entityName) const;
    Q_INVOKABLE EntityComponentMap *getEntityById(int t_id) const;
    Q_INVOKABLE QList<int> getEntityList() const;

    static VeinQml *getStaticInstance();
    static void setStaticInstance(VeinQml *t_instance);

    Q_INVOKABLE void entitySubscribeById(int t_entityId);
    Q_INVOKABLE void entityUnsubscribeById(int t_entityId);

signals:
    void sigStateChanged(ConnectionState t_state);
    void sigEntityAvailable(QString t_entityName);

private slots:
    void onEntityLoaded(int t_entityId);
private:
    int idFromEntityName(const QString &t_entityName) const;
    QString nameFromEntityId(int t_entityId) const;
    void removeEntity(int t_entityId);

    ConnectionState m_state = ConnectionState::VQ_IDLE;
    QHash<int, EntityComponentMap *> m_entities;
    QHash<int, quint32> m_entitySubscriptionReferenceTables;
    QSet<int> m_resolvedIds;
    // do not delete from c++
    static VeinQml *s_staticInstance;
};
}
#endif // VEINQML_H
