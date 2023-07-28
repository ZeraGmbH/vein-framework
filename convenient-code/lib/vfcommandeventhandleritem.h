#ifndef VFCOMMANDEVENTHANDLERITEM_H
#define VFCOMMANDEVENTHANDLERITEM_H

#include "ve_eventsystem.h"
#include "ve_commandevent.h"
#include <memory>

class VfCommandEventHandlerItem
{
public:
    VfCommandEventHandlerItem(int entityId);
    void setEventSystem(VeinEvent::EventSystem* eventSystem);
protected:
    VeinEvent::EventSystem* m_eventSystem = nullptr;
    const int m_entityId;
private:
    friend class VfCommandEventHandler;
    int getEntityId();
    virtual void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) = 0;
};

typedef std::shared_ptr<VfCommandEventHandlerItem> VfCommandEventHandlerItemPtr;

#endif // VFCOMMANDEVENTHANDLERITEM_H
