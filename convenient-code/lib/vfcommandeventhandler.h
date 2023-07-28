#ifndef VFCOMMANDEVENTHANDLER_H
#define VFCOMMANDEVENTHANDLER_H

#include <vfcmdeventitem.h>
#include <QHash>
#include <set>

class VfCommandEventHandler
{
public:
    VfCommandEventHandler(VeinEvent::CommandEvent::EventSubtype eventSubtypeFilter = VeinEvent::CommandEvent::EventSubtype::NOTIFICATION);
    void addItem(VfCmdEventItemPtr item);
    void removeItem(VfCmdEventItemPtr item);
    void processEvent(QEvent *event);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent);
private:
    QHash<int, std::set<VfCmdEventItemPtr>> m_items;
    const VeinEvent::CommandEvent::EventSubtype m_eventSubtypeFilter;
};

#endif // VFCOMMANDEVENTHANDLER_H
