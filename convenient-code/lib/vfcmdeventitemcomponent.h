#ifndef VFCMDEVENTITEMCOMPONENT_H
#define VFCMDEVENTITEMCOMPONENT_H

#include "vfcmdeventitem.h"
#include "vcmp_componentdata.h"

class VfCmdEventItemComponent : public VfCmdEventItem
{
public:
    VfCmdEventItemComponent(int entityId, QString componentName);
protected:
    QString m_componentName;
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    virtual void processComponentEventData(const VeinComponent::ComponentData *cData) = 0;
};

#endif // VFCMDEVENTITEMCOMPONENT_H
