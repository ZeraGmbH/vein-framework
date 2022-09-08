#ifndef VEINEVENT_VE_VALIDATOR_H
#define VEINEVENT_VE_VALIDATOR_H

#include "vfevent_export.h"
#include "globalIncludes.h"
#include "ve_eventsystem.h"

namespace VeinEvent
{
  /**
   * @brief A basic validator that intercepts transactions and turns them into notifications
   *
   * The default implementation may be too simple in most usecases
   * Converts the given event of type CommandEvent with subtype "transaction" into subtype "notification"
   * Validators in general should consume invalid events via QEvent::accept to prevent propagation to other event systems
   */
  class VFEVENT_EXPORT Validator : public EventSystem
  {
    Q_OBJECT
  public:
    explicit Validator(QObject *t_parent = nullptr);

    // EventSystem interface
  public:
    bool processEvent(QEvent *t_event) override;

  signals:

  public slots:

  };

} // namespace VeinEvent

#endif // VEINEVENT_VE_VALIDATOR_H
