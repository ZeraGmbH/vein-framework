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
   */
class VFEVENT_EXPORT EventSystem : public QObject
{
    Q_OBJECT
public:
    explicit EventSystem(QObject *parent = nullptr);
    ~EventSystem();
    virtual void processEvent(QEvent *event) = 0;
    void attach(EventHandler *eventHandler);
    void detach();
    static int getInstanceCount();

signals:
    void sigSendEvent(QEvent *event);
    void sigAttached();
private slots:
    void onSendEvent(QEvent *event);

private:
    EventHandler *m_eventHandler = nullptr;
    static int m_instanceCount;
};
}
#endif // EVENTSYSTEM_H
