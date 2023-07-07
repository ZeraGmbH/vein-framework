#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "vfevent_export.h"
#include "globalIncludes.h"
#include <QObject>

class QEvent;

namespace VeinEvent
{
class EventHandler;
/**
   * @brief Interface for event systems that can be attached to VeinEvent::EventHandler
   * @note if you want to capture events, eg. for replay, please note that the QEvent::type (see http://doc.qt.io/qt-5/qevent.html#type) of most events is not initialized in a deterministic manner
   * @todo idea: add template specialized objects that handle one particular type of event via lambda infused code
   */
class VFEVENT_EXPORT EventSystem : public QObject
{
    Q_OBJECT

public:
    explicit EventSystem(QObject *t_parent=nullptr);
    ~EventSystem() {}

    /**
     * @brief Processes the given event
     * @param t_event
     * @note If the event is accepted it counts as consumed and will not be processed further
     */
    virtual void processEvent(QEvent *t_event)=0;

    /**
     * @brief Connects the sigSendEvent signal with the EventHandler
     * @param t_eventHandler [in]
     */
    void attach(EventHandler *t_eventHandler);

signals:
    /**
     * @brief Forwards events to the attached EventHandler
     * @param t_event
     */
    void sigSendEvent(QEvent *t_event);
    /**
     * @brief Is called when the system is attached to an EventHandler
     */
    void sigAttached();

private:
    bool m_attached = false;
};
}
#endif // EVENTSYSTEM_H
