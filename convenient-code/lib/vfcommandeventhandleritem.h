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
    int getEntityId() const;
private:
    friend class VfCommandEventHandler;
    virtual void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) = 0;
    const int m_entityId;
};

typedef std::shared_ptr<VfCommandEventHandlerItem> VfCommandEventHandlerItemPtr;

#endif // VFCOMMANDEVENTHANDLERITEM_H
