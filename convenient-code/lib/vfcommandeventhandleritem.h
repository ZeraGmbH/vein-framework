#ifndef VFCOMMANDEVENTHANDLERITEM_H
#define VFCOMMANDEVENTHANDLERITEM_H

#include "ve_eventsystem.h"
#include "ve_commandevent.h"
#include <memory>

class VfCommandEventHandlerItem
{
public:
    VfCommandEventHandlerItem(int entityId);
    int getEntityId();
    void setEventSystem(VeinEvent::EventSystem* eventSystem);
    virtual void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) = 0;
protected:
    int m_entityId;
    VeinEvent::EventSystem* m_eventSystem = nullptr;
};

typedef std::shared_ptr<VfCommandEventHandlerItem> VfCommandEventHandlerItemPtr;

#endif // VFCOMMANDEVENTHANDLERITEM_H
