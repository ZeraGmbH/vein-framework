#ifndef VFCOMMANDEVENTHANDLERSYSTEM_H
#define VFCOMMANDEVENTHANDLERSYSTEM_H

#include "vfcommandeventhandler.h"
#include "ve_eventsystem.h"

class VfCommandEventHandlerSystem : public VeinEvent::EventSystem
{
public:
    void addItem(VfCommandEventHandlerItemPtr item);
    void removeItem(VfCommandEventHandlerItemPtr item);
private:
    void processEvent(QEvent *event) override;
    VfCommandEventHandler m_commandEventHandler;
};

#endif // VFCOMMANDEVENTHANDLERSYSTEM_H
