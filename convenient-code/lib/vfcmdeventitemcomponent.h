#ifndef VFCMDEVENTITEMCOMPONENT_H
#define VFCMDEVENTITEMCOMPONENT_H

#include "vfcmdeventitem.h"
#include "vcmp_componentdata.h"

class VfCmdEventItemComponent : public VfCmdEventItem
{
public:
    VfCmdEventItemComponent(int entityId, QString componentName);
    const QString &getComponentName() const;
    virtual void processComponentEventData(const VeinComponent::ComponentData *cData) = 0;
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    QString m_componentName;
};

typedef std::shared_ptr<VfCmdEventItemComponent> VfCmdEventItemComponentPtr;

#endif // VFCMDEVENTITEMCOMPONENT_H
