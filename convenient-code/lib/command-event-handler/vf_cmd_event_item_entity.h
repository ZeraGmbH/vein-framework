#ifndef VFCMDEVENTITEMENTITY_H
#define VFCMDEVENTITEMENTITY_H

#include "vf_entity_event_item.h"
#include "container_safe_delete_while_loop.h"

class VfComponentEventItem; // forward / repeat typedef to avoid circular include
typedef std::shared_ptr<VfComponentEventItem> VfCmdEventItemComponentPtr;


// Receive command events and distribute component event data to VfComponentEventItem objects
class VfCmdEventItemEntity : public VfEntityEventItem
{
public:
    static std::shared_ptr<VfCmdEventItemEntity> create(int entityId);
    VfCmdEventItemEntity(int entityId);

    void addItem(VfCmdEventItemComponentPtr item);
    void removeItem(VfCmdEventItemComponentPtr item);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
private:
    typedef ContainerSafeDeleteWhileLoop<VfCmdEventItemComponentPtr, std::unordered_set<VfCmdEventItemComponentPtr>> SafeDeleteSet;
    QHash<QString, SafeDeleteSet> m_componentItems;
};

typedef std::shared_ptr<VfCmdEventItemEntity> VfCmdEventItemEntityPtr;

#endif // VFCMDEVENTITEMENTITY_H
