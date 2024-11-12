#ifndef ENTITYCOMPONENTMAP_H
#define ENTITYCOMPONENTMAP_H

#include "globalIncludes.h"
#include <QQmlPropertyMap>
#include <QJSValue>
#include <QVariantMap>
#include <QUuid>

namespace VeinComponent
{
  class ComponentData;
  class RemoteProcedureData;
}

namespace VeinApiQml
{
  class VFQML_EXPORT EntityComponentMap : public QQmlPropertyMap
  {
    Q_OBJECT

    /// @b away with the stupid default constructor
    EntityComponentMap() = delete;

  public:
    explicit EntityComponentMap(int entityId, const QVariantHash &entityIntrospection, QObject *parent=nullptr);

    enum class DataState : int {
      ECM_NONE = -1, /**< uninitialized */
      ECM_PENDING = 0, /**< introspection is available but values are not initialized */
      ECM_READY = 1, /**< everything is available */
      ECM_REMOVED = 2, /**< the entity has been removed from the remote end */
    };
    Q_ENUM(DataState)

    Q_PROPERTY(DataState state READ state NOTIFY sigStateChanged)
    Q_PROPERTY(QStringList remoteProcedures READ getRemoteProcedureList NOTIFY sigRemoteProceduresChanged)

    void processComponentData(const VeinComponent::ComponentData *cData);
    void processRemoteProcedureData(const VeinComponent::RemoteProcedureData *rpcData);

    DataState state() const;
    /**
     * @brief calls loadEntityData() if the t_dataState is ECM_PENDING
     * @param t_dataState
     * @note not callable from QML
     */
    void setState(DataState t_dataState);

    Q_INVOKABLE int entityId() const;
    //alias for QQmlPropertyMap::contains
    Q_INVOKABLE bool hasComponent(const QString &componentName) const;
    Q_INVOKABLE int propertyCount() const;
    /**
     * @brief Calls remote procedure
     * @param t_procedureName
     * @param t_parameters
     * @return id of the result to expect
     */
    Q_INVOKABLE QUuid invokeRPC(const QString &procedureName, const QVariantMap &parameters);
    Q_INVOKABLE void cancelRPCInvokation(QUuid identifier);

    Q_INVOKABLE QList<QString> getRemoteProcedureList() const;

  signals:
    void sigSendEvent(QEvent *t_cEvent);
    void sigEntityComplete(int t_entityId);

    void sigStateChanged(DataState t_state);
    void sigRPCFinished(QUuid identifier, const QVariantMap &resultData);
    void sigRPCProgress(QUuid t_identifier, const QVariantMap &t_progressData);
    void sigRemoteProceduresChanged(QStringList t_procedureList);

  protected:
    /**
     * @brief Intercepts all value changes coming from the qml side and converts them into CommandEvents
     * @note updateValue is NOT called when changes are made by calling insert() or clear() - it is only emitted when a value is updated from QML.
     * @return returns the current value (not the changed one) which will not trigger a valueChanged until the change notification arrives
     */
    QVariant updateValue(const QString &t_key, const QVariant &t_newValue) override;

  private:
    void loadEntityData();
    QList<QString> m_pendingValues;
    QList<QString> m_registeredRemoteProcedures;
    QHash<QUuid, QString> m_pendingRPCCallbacks;
    const QVariantHash m_entityIntrospection;
    DataState m_state = DataState::ECM_PENDING;
    const int m_entityId;
  };
}
#endif // ENTITYCOMPONENTMAP_H
