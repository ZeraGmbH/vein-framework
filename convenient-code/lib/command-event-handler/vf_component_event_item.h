#ifndef VFCMDEVENTITEMCOMPONENT_H
#define VFCMDEVENTITEMCOMPONENT_H

#include "vf_cmd_event_item_entity.h"
#include "vcmp_componentdata.h"

class VfComponentEventItem
{
public:
    VfComponentEventItem(QString componentName, VfCmdEventItemEntityPtr entityItem);
    int getEntityId() const;
    const QString &getComponentName() const;
    void sendEvent(QEvent *event) const;
    virtual void processComponentEventData(const VeinComponent::ComponentData *componentData) = 0;
    virtual void processErrorComponentEventData(const VeinComponent::ComponentData *originalComponentData) = 0;
private:
    const QString m_componentName;
    VfCmdEventItemEntityPtr m_entityItem;
};

typedef std::shared_ptr<VfComponentEventItem> VfCmdEventItemComponentPtr;

#endif // VFCMDEVENTITEMCOMPONENT_H
