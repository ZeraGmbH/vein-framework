#ifndef VFCMDEVENTITEMENTITY_H
#define VFCMDEVENTITEMENTITY_H

#include "vfcmdeventitem.h"
#include "containersafedeletewhileloop.h"

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
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    QHash<QString, CmdEventItemEntityContainerType> m_componentItems;
};

typedef std::shared_ptr<VfCmdEventItemEntity> VfCmdEventItemEntityPtr;

#endif // VFCMDEVENTITEMENTITY_H
