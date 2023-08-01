#ifndef VFCOMMANDEVENTHANDLER_H
#define VFCOMMANDEVENTHANDLER_H

#include "vf_cmd_event_item.h"
#include "container_safe_delete_while_loop.h"
#include <QHash>

typedef ContainerSafeDeleteWhileLoop<VfCmdEventItemPtr, std::unordered_set<VfCmdEventItemPtr>> CommandEventHandlerContainerType;

class VfCmdEventHandler
{
public:
    VfCmdEventHandler(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);

    // Interface for VfCmdEventHandlerSystem
    void addItem(VfCmdEventItemPtr item);
    void removeItem(VfCmdEventItemPtr item);
    void processEvent(QEvent *event);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent);
    void handleErrorData(VeinEvent::EventData *&eventData, QHash<int, CommandEventHandlerContainerType>::iterator iter);
    QHash<int, CommandEventHandlerContainerType> m_items;
    const VeinEvent::CommandEvent::EventSubtype m_eventSubtypeFilter;
};

#endif // VFCOMMANDEVENTHANDLER_H
