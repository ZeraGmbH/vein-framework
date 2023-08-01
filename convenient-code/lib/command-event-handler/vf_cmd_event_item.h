#ifndef VFCMDEVENTITEM_H
#define VFCMDEVENTITEM_H

#include "ve_eventsystem.h"
#include "ve_commandevent.h"
#include <memory>

// Base item handled by VfCmdEventHandler
class VfCmdEventItem
{
public:
    VfCmdEventItem(int entityId);
    VeinEvent::EventSystem* getEvenSystem() const;
    void setEventSystem(VeinEvent::EventSystem* eventSystem);
    int getEntityId() const;
    virtual void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) = 0;
    virtual void processErrorCommandEventData(VeinEvent::EventData *originalEventData) = 0;
private:
    const int m_entityId;
    VeinEvent::EventSystem* m_eventSystem = nullptr;
};

typedef std::shared_ptr<VfCmdEventItem> VfCmdEventItemPtr;

#endif // VFCMDEVENTITEM_H
