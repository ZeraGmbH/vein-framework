#ifndef VFCMDEVENTITEMENTITY_H
#define VFCMDEVENTITEMENTITY_H

#include "vfcmdeventitem.h"
#include <set>

class VfCmdEventItemComponent;
typedef std::shared_ptr<VfCmdEventItemComponent> VfCmdEventItemComponentPtr;

class VfCmdEventItemEntity : public VfCmdEventItem
{
public:
    static std::shared_ptr<VfCmdEventItemEntity> create(int entityId);
    VfCmdEventItemEntity(int entityId);
    void addItem(VfCmdEventItemComponentPtr item);
    void removeItem(VfCmdEventItemComponentPtr item);
private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    QHash<QString, std::set<VfCmdEventItemComponentPtr>> m_componentItems;
};

typedef std::shared_ptr<VfCmdEventItemEntity> VfCmdEventItemEntityPtr;

#endif // VFCMDEVENTITEMENTITY_H
