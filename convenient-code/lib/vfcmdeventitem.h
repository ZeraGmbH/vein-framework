#ifndef VFCMDEVENTITEM_H
#define VFCMDEVENTITEM_H

#include "ve_eventsystem.h"
#include "ve_commandevent.h"
#include <memory>

class VfCmdEventItem
{
public:
    VfCmdEventItem(int entityId);
    void setEventSystem(VeinEvent::EventSystem* eventSystem);
protected:
    VeinEvent::EventSystem* m_eventSystem = nullptr;
    int getEntityId() const;
private:
    friend class VfCommandEventHandler;
    virtual void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) = 0;
    const int m_entityId;
};

typedef std::shared_ptr<VfCmdEventItem> VfCmdEventItemPtr;

#endif // VFCMDEVENTITEM_H
