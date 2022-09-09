#include "vftesteventhandler.h"

VfTestEventHandler::VfTestEventHandler(QList<VeinEvent::EventSystem *> eventSystems)
{
    for(const auto& eventSystem : qAsConst(eventSystems)) {
        addSubsystem(eventSystem);
    }
}
