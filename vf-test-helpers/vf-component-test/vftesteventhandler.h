#ifndef VFTESTEVENTHANDLER_H
#define VFTESTEVENTHANDLER_H

#include <ve_eventhandler.h>
#include <ve_eventsystem.h>
#include <QList>

class VfTestEventHandler : public VeinEvent::EventHandler
{
public:
    VfTestEventHandler(QList<VeinEvent::EventSystem*> eventSystems);
};

#endif // VFTESTEVENTHANDLER_H
