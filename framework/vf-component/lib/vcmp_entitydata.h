#ifndef VEINEVENT_VE_ENTITYEVENT_H
#define VEINEVENT_VE_ENTITYEVENT_H

#include "vfcomponent_export.h"
#include "globalIncludes.h"
#include <ve_eventdata.h>

namespace VeinComponent
{
  /**
   * @brief Command object (see GoF Command pattern) for entity data
   */
  class VFCOMPONENT_EXPORT EntityData : public VeinEvent::EventData
  {
  public:
    enum class Command : qint8 {
      ECMD_INVALID = -1, /**< default */
      ECMD_ADD = 0, /**< adds a new entity */
      ECMD_REMOVE = 1, /**< removes the entity */
      ECMD_SUBSCRIBE = 2, /**< starts async notifications */
      ECMD_UNSUBSCRIBE = 3 /**< stops async notifications */
    };

    explicit EntityData();

    /**
     * @brief The dataType is a unique identifier for this type of EventData
     * @return
     */
    constexpr static int dataType() { return VCMP_ENTITYDATA_DATATYPE; }

    void setCommand(Command t_eDataCommand);
    Command eventCommand() const;


    // EventData interface
  public:
    bool isValid() const override;
    int type() const override { return VCMP_ENTITYDATA_DATATYPE; }
    QByteArray serialize() const override;
    void deserialize(const QByteArray &t_data) override;



  private:
    Command m_command = Command::ECMD_INVALID;
  };

} // namespace VeinEvent

#endif // VEINEVENT_VE_ENTITYEVENT_H
