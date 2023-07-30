#ifndef VFCMDEVENTITEMCOMPONENT_H
#define VFCMDEVENTITEMCOMPONENT_H

#include "vfcmdeventitementity.h"
#include "ve_eventsystem.h"
#include "vcmp_componentdata.h"

class VfCmdEventItemComponent
{
public:
    VfCmdEventItemComponent(QString componentName, VfCmdEventItemEntityPtr entityItem);
    const QString &getComponentName() const;
    VfCmdEventItemEntityPtr getEntityItem() const;
    VeinEvent::EventSystem* getEvenSystem() const;
    virtual void processComponentEventData(const VeinComponent::ComponentData *componentData) = 0;
    virtual void processErrorComonentEventData(const VeinComponent::ComponentData *originalComponentData) = 0;
private:
    const QString m_componentName;
    VfCmdEventItemEntityPtr m_entityItem;
};

typedef std::shared_ptr<VfCmdEventItemComponent> VfCmdEventItemComponentPtr;

#endif // VFCMDEVENTITEMCOMPONENT_H
