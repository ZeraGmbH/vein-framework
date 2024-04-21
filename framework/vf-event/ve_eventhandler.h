#ifndef VEINEVENTHANDLER_H
#define VEINEVENTHANDLER_H

#include "vfevent_export.h"
#include <timertemplateqt.h>
#include <QObject>
#include <QList>

namespace VeinEvent
{
class EventSystem;

/**
   * @brief A simple event handler for QEvent based types, operates with VeinEvent::EventSystem
   * @note if you want to capture events, eg. for replay, please note that the QEvent::type (see http://doc.qt.io/qt-5/qevent.html#type) of most events is not initialized in a deterministic manner
   */
class VFEVENT_EXPORT EventHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<EventSystem*> subsystems READ subsystems WRITE setSubsystems NOTIFY subsystemsChanged)
public:
    explicit EventHandler(QObject *parent=nullptr);
    ~EventHandler();
    QList<EventSystem*> subsystems() const;
public slots:
    // EventHandler is not owner and therefore not responsible for deleting subsystems
    void setSubsystems(QList<EventSystem*> subsystems);
    void addSubsystem(VeinEvent::EventSystem* subsystem);
    void prependSubsystem(VeinEvent::EventSystem* subsystem);
    void removeSubsystem(VeinEvent::EventSystem* subsystem);
    void clearSubsystems();
signals:
    void subsystemsChanged(QList<EventSystem*> subsystems);
    void sigEventAccepted(VeinEvent::EventSystem* eventSystem, QEvent *event);
protected:
    virtual void customEvent(QEvent *event) override;
    TimerTemplateQtPtr m_waitForAddSilenceToLogTimer;
private:
    void onAddSilence();
    /**
     * @brief Ordered list of event system that is reprenstable for the event processing order
     */
    QList<EventSystem*> m_subsystems;
};
}
#endif // VEINEVENTHANDLER_H
