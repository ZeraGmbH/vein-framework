#ifndef VFCOMMANDEVENTHANDLER_H
#define VFCOMMANDEVENTHANDLER_H

#include <vfcommandeventhandleritem.h>
#include <QEvent>
#include <QHash>
#include <set>

class VfCommandEventHandler
{
public:
    void addItem(VfCommandEventHandlerItemPtr item);
    void removeItem(VfCommandEventHandlerItemPtr item);
    bool containsItem(int entityId);
    void processEvent(QEvent *event);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent);
private:
    QHash<int, std::set<VfCommandEventHandlerItemPtr>> m_items;
};

#endif // VFCOMMANDEVENTHANDLER_H
