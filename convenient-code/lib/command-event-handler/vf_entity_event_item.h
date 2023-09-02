#ifndef VFCMDEVENTITEM_H
#define VFCMDEVENTITEM_H

#include "ve_eventsystem.h"
#include "ve_commandevent.h"
#include "container_safe_delete_while_loop.h"
#include <unordered_set>
#include <memory>

class VfEntityEventItem;
typedef std::shared_ptr<VfEntityEventItem> VfCmdEventItemPtr;

// Base item handled by VfCmdEventHandler
class VfEntityEventItem
{
public:
    VfEntityEventItem(int entityId);
    VeinEvent::EventSystem* getEventSystem() const;
    void setEventSystem(VeinEvent::EventSystem* eventSystem);
    int getEntityId() const;
    virtual void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) = 0;
    virtual void processErrorCommandEventData(VeinEvent::EventData *originalEventData) = 0;
private:
    typedef ContainerSafeDeleteWhileLoop<VfCmdEventItemPtr, std::unordered_set<VfCmdEventItemPtr>> UnorderedSetSafeDelete;
    const int m_entityId;
    VeinEvent::EventSystem* m_eventSystem = nullptr;
};


#endif // VFCMDEVENTITEM_H
