#ifndef VFCOMMANDEVENTHANDLERCOMPONENTITEM_H
#define VFCOMMANDEVENTHANDLERCOMPONENTITEM_H

#include "vfcommandeventhandleritem.h"
#include "vcmp_componentdata.h"

class VfCommandEventHandlerComponentItem : public VfCommandEventHandlerItem
{
public:
    VfCommandEventHandlerComponentItem(int entityId, QString componentName);
protected:
    QString m_componentName;
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    virtual void processComponentEventData(const VeinComponent::ComponentData *cData) = 0;
};

#endif // VFCOMMANDEVENTHANDLERCOMPONENTITEM_H
