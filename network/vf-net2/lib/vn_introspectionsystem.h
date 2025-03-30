#ifndef VEINNET_INTROSPECTIONSYSTEM_H
#define VEINNET_INTROSPECTIONSYSTEM_H

#include <ve_eventsystem.h>
#include "vfnet2_export.h"
#include "vs_abstracteventsystem.h"

namespace VeinNet
{

class VFNET2_EXPORT IntrospectionSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit IntrospectionSystem(VeinStorage::AbstractEventSystem *storageSystem, QObject *parent=nullptr);
    static const QString s_nameComponent;

    void processEvent(QEvent *event) override;
private:
    QStringList getRpcNames(int entityId) const;
    VeinStorage::AbstractEventSystem *m_storageSystem = nullptr;
};

}

#endif // VEINNET_INTROSPECTIONSYSTEM_H
