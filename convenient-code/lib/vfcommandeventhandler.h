#ifndef VFCOMMANDEVENTHANDLER_H
#define VFCOMMANDEVENTHANDLER_H

#include <vfcommandeventhandleritem.h>
#include <QHash>
#include <set>

class VfCommandEventHandler
{
public:
    VfCommandEventHandler(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);
    void addItem(VfCommandEventHandlerItemPtr item);
    void removeItem(VfCommandEventHandlerItemPtr item);
    void processEvent(QEvent *event);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent);
private:
    QHash<int, std::set<VfCommandEventHandlerItemPtr>> m_items;
    const VeinEvent::CommandEvent::EventSubtype m_eventSubtypeFilter;
};

#endif // VFCOMMANDEVENTHANDLER_H
