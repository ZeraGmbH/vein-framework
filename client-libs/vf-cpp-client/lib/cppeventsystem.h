#ifndef CPPEVENTSYSTEM_H
#define CPPEVENTSYSTEM_H

#include "cppentitycomponentmap.h"
#include <ve_eventsystem.h>
#include <memory>

class CppEventSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    CppEventSystem();
    enum class ConnectionState : int {
      VQ_IDLE = 0, /**< the system has been created and is not yet ready */
      VQ_LOADED = 1, /**< the required entities are set and their introspection data has been fetched */
      VQ_DISCONNECTED = 2, /**< the host or the client has closed the connection */
      VQ_ERROR = 3 /**< error state, e.g. a required entity is not available on the server */
    };

    ConnectionState state() const;
    CppEntityComponentMapPtr getEntity(const QString &entityName) const;
    bool hasEntity(const QString &entityName) const;
    CppEntityComponentMapPtr getEntityById(int id) const;
    QList<int> getEntityList() const;
    void entitySubscribeById(int entityId);
    void entityUnsubscribeById(int entityId);
    bool processEvent(QEvent *event) override;
signals:
    void sigStateChanged(CppEventSystem::ConnectionState state);
    void sigEntityAvailable(QString entityName);
private slots:
    void onEntityLoaded(int entityId);
private:
    void addEntity(CppEntityComponentMapPtr entity, int id, const QString &name);

    QHash<int, CppEntityComponentMapPtr> m_entities;
    QHash<QString, CppEntityComponentMapPtr> m_entitiesStr;
    ConnectionState m_state = ConnectionState::VQ_IDLE;
    QHash<int, quint32> m_entitySubscriptionReferenceTables;
    QSet<int> m_resolvedIds;
};

#endif // CPPEVENTSYSTEM_H
