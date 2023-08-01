#ifndef VFCOMMANDEVENTHANDLERSYSTEM_H
#define VFCOMMANDEVENTHANDLERSYSTEM_H

#include "vfcommandeventhandler.h"
#include "ve_eventsystem.h"
#include <memory>

class VfCommandEventHandlerSystem : public VeinEvent::EventSystem
{
public:
    static std::shared_ptr<VfCommandEventHandlerSystem> create(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);
    VfCommandEventHandlerSystem(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);

    void addItem(VfCmdEventItemPtr item);
    void removeItem(VfCmdEventItemPtr item);
    void processEvent(QEvent *event) override;
private:
    VfCommandEventHandler m_commandEventHandler;
};

typedef std::shared_ptr<VfCommandEventHandlerSystem> VfCommandEventHandlerSystemPtr;

#endif // VFCOMMANDEVENTHANDLERSYSTEM_H
