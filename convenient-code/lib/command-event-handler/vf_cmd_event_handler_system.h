#ifndef VFCOMMANDEVENTHANDLERSYSTEM_H
#define VFCOMMANDEVENTHANDLERSYSTEM_H

#include "vf_cmd_event_handler.h"
#include "ve_eventsystem.h"
#include <memory>

class VfCmdEventHandlerSystem : public VeinEvent::EventSystem
{
public:
    static std::shared_ptr<VfCmdEventHandlerSystem> create(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);
    VfCmdEventHandlerSystem(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);

    void addItem(VfCmdEventItemPtr item);
    void removeItem(VfCmdEventItemPtr item);
    void processEvent(QEvent *event) override;
private:
    VfCmdEventHandler m_commandEventHandler;
};

typedef std::shared_ptr<VfCmdEventHandlerSystem> VfCmdEventHandlerSystemPtr;

#endif // VFCOMMANDEVENTHANDLERSYSTEM_H
