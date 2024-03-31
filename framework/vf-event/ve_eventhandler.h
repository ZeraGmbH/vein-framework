#ifndef VEINEVENTHANDLER_H
#define VEINEVENTHANDLER_H

#include "vfevent_export.h"
#include <QObject>
#include <QList>

namespace VeinEvent
{
class EventSystem;

/**
   * @brief A simple event handler for QEvent based types, operates with VeinEvent::EventSystem
   * @note if you want to capture events, eg. for replay, please note that the QEvent::type (see http://doc.qt.io/qt-5/qevent.html#type) of most events is not initialized in a deterministic manner
   * @todo maybe implement specialized groups of systems with the strategy pattern, or in a hash table with QEvent::type() as key
   */
class VFEVENT_EXPORT EventHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<EventSystem*> subsystems READ subsystems WRITE setSubsystems NOTIFY subsystemsChanged)
public:
    explicit EventHandler(QObject *parent=nullptr);
    ~EventHandler() {m_subsystems.clear();}
    QList<EventSystem*> subsystems() const;
public slots:
    /**
     * @brief Sets up the list of EventSystem
     * @warning the EventHandler is not responsible for deleting subsystems
     */
    void setSubsystems(QList<EventSystem*> subsystems);
    void addSubsystem(VeinEvent::EventSystem* subsystem);
    void clearSubsystems();
signals:
    void subsystemsChanged(QList<EventSystem*> subsystems);
    void eventAccepted(VeinEvent::EventSystem* eventSystem, QEvent *event);
protected:
    virtual void customEvent(QEvent *event) override;
private:
    /**
     * @brief Ordered list of event system that is reprenstable for the event processing order
     * @todo add partitioning of event systems by event type to reduce overhead in event processing
     */
    QList<EventSystem*> m_subsystems;
};
}
#endif // VEINEVENTHANDLER_H
