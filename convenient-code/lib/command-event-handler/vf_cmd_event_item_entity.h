#ifndef VFCMDEVENTITEMENTITY_H
#define VFCMDEVENTITEMENTITY_H

#include "vf_cmd_event_item.h"
#include "container_safe_delete_while_loop.h"

class VfCmdEventItemComponent;
typedef std::shared_ptr<VfCmdEventItemComponent> VfCmdEventItemComponentPtr;
typedef ContainerSafeDeleteWhileLoop<VfCmdEventItemComponentPtr, std::unordered_set<VfCmdEventItemComponentPtr>> CmdEventItemEntityContainerType;


class VfCmdEventItemEntity : public VfCmdEventItem
{
public:
    static std::shared_ptr<VfCmdEventItemEntity> create(int entityId);
    VfCmdEventItemEntity(int entityId);

    void addItem(VfCmdEventItemComponentPtr item);
    void removeItem(VfCmdEventItemComponentPtr item);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
private:
    QHash<QString, CmdEventItemEntityContainerType> m_componentItems;
};

typedef std::shared_ptr<VfCmdEventItemEntity> VfCmdEventItemEntityPtr;

#endif // VFCMDEVENTITEMENTITY_H
