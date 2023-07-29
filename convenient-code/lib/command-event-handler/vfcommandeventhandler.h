#ifndef VFCOMMANDEVENTHANDLER_H
#define VFCOMMANDEVENTHANDLER_H

#include "vfcmdeventitem.h"
#include "containersafedeletewhileloop.h"
#include <QHash>

class VfCommandEventHandler
{
public:
    VfCommandEventHandler(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);
    void addItem(VfCmdEventItemPtr item);
    void removeItem(VfCmdEventItemPtr item);
    void processEvent(QEvent *event);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent);
private:
    QHash<int, ContainerSafeDeleteWhileLoop<VfCmdEventItemPtr>> m_items;
    const VeinEvent::CommandEvent::EventSubtype m_eventSubtypeFilter;
};

#endif // VFCOMMANDEVENTHANDLER_H
