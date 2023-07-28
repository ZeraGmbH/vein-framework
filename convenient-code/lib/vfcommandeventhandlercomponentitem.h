#ifndef VFCOMMANDEVENTHANDLERCOMPONENTITEM_H
#define VFCOMMANDEVENTHANDLERCOMPONENTITEM_H

#include "vfcommandeventhandleritem.h"

class VfCommandEventHandlerComponentItem : public VfCommandEventHandlerItem
{
public:
    VfCommandEventHandlerComponentItem(int entityId, QString componentName);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    virtual void processComponentCommandEvent(VeinEvent::CommandEvent *cmdEvent) = 0;
    QString m_componentName;
};

#endif // VFCOMMANDEVENTHANDLERCOMPONENTITEM_H
