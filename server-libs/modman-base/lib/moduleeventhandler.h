#ifndef MODULEEVENTHANDLER_H
#define MODULEEVENTHANDLER_H

#include <ve_eventhandler.h>
#include <ve_eventsystem.h>
#include <QList>

class ModuleEventHandler : public VeinEvent::EventHandler
{
    Q_OBJECT
public:
    explicit ModuleEventHandler(QObject *parent = nullptr);
    void addModuleSystem(VeinEvent::EventSystem *eventSystem);
    void clearModuleSystems();
protected:
    void customEvent(QEvent *event) override;
private:
    // Modules' event systems are processed first
    QList<VeinEvent::EventSystem *> m_moduleSystems;
};

#endif // MODULEEVENTHANDLER_H
