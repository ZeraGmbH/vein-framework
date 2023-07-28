#ifndef VFCOMMANDEVENTHANDLERSYSTEM_H
#define VFCOMMANDEVENTHANDLERSYSTEM_H

#include "vfcommandeventhandler.h"
#include "ve_eventsystem.h"

class VfCommandEventHandlerSystem : public VeinEvent::EventSystem
{
public:
    VfCommandEventHandlerSystem(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);
    void addItem(VfCmdEventItemPtr item);
    void removeItem(VfCmdEventItemPtr item);
private:
    void processEvent(QEvent *event) override;
    VfCommandEventHandler m_commandEventHandler;
};

#endif // VFCOMMANDEVENTHANDLERSYSTEM_H
