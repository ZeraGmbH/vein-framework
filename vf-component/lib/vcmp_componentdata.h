#ifndef ENTITYEVENT_H
#define ENTITYEVENT_H

#include "vfcomponent_export.h"
#include "globalIncludes.h"
#include <ve_eventdata.h>

class QVariant;

/**
 * @brief Namespace for QEvent based custom event types to be used with VeinEvent
 */
namespace VeinComponent
{
  /**
   * @brief Command object (see GoF Command pattern) for component data
   */
  class VFCOMPONENT_EXPORT ComponentData : public VeinEvent::EventData
  {
  public:
    enum class Command : qint8 {
      CCMD_INVALID = -1, /**< default */
      CCMD_ADD = 0, /**< adds a new component with the value given or fails if the component already exists */
      CCMD_REMOVE = 1, /**< removes the value entirely, currently this command never fails */
      CCMD_SET = 2, /**< sets a value or fails when the component does not exist */
      CCMD_FETCH = 3, /**< retrieves a value or fails if the component does not exist */
    };

    explicit ComponentData();
    explicit ComponentData(int t_entityId, Command t_cDataCommand=Command::CCMD_SET);
    ComponentData(const ComponentData &t_other);
    /**
     * @brief The dataType is a unique identifier for this type of EventData
     * @return
     */
    static constexpr int dataType() { return VCMP_COMPONENTDATA_DATATYPE; }

    Command eventCommand() const;
    void setCommand(Command t_cDataCommand);

    const QString &componentName() const;
    void setComponentName(const QString &t_componentName);

    /**
     * @brief The changed data this event propagates
     * @return may be invalid in case of no value is set or Command is REMOVE
     */
    const QVariant &newValue() const;
    void setNewValue(const QVariant &t_newValue);

    /**
     * @brief The current data expected by the sender of this event
     * @return may be invalid when no previous value was set
     * @note if the currently stored value differs from oldValue() then the request is invalid (except in the CCMD_FETCH case)
     */
    const QVariant &oldValue() const;
    void setOldValue(const QVariant &t_oldValue);


    // EventData interface
  public:
    bool isValid() const override;
    QByteArray serialize() const override;
    void deserialize(const QByteArray &t_data) override;
    int type() const override { return VCMP_COMPONENTDATA_DATATYPE; }

  private:
    /**
     * @brief the specific command for this event
     */
    Command m_command = Command::CCMD_INVALID;

    /**
     * @brief the name of the component, unique under the scope of the entityId
     */
    QString m_componentName;

    /**
     * @brief the value to update the current value
     */
    QVariant m_newValue;

    /**
     * @brief the value that is assumed to be current
     * @note in case of mismatch a transaction event containing this data will fail
     */
    QVariant m_oldValue;
  };
}

#endif // ENTITYEVENT_H
