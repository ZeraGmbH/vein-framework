#ifndef VEINNET_INTROSPECTIONSYSTEM_H
#define VEINNET_INTROSPECTIONSYSTEM_H

#include <ve_eventsystem.h>
#include "vs_abstracteventsystem.h"
#include "globalIncludes.h"
#include <QHash>
#include <QJsonObject>

namespace VeinNet
{

struct EntityIntrospection;

/**
* @brief Collates VeinStorage data structure informations into VeinComponent::IntrospectionData for remote introspection
*/
class VFNET2_EXPORT IntrospectionSystem : public VeinEvent::EventSystem
{
    Q_OBJECT
public:
    explicit IntrospectionSystem(VeinStorage::AbstractEventSystem *storageSystem, QObject *parent=nullptr);
    static const QString s_nameComponent;

    void processEvent(QEvent *event) override;
private:
    QStringList getRpcNames(int entityId) const;
    QHash<int, QStringList> m_rpcEntityNames;
    VeinStorage::AbstractEventSystem *m_storageSystem = nullptr;
};

} // namespace VeinNet

#endif // VEINNET_INTROSPECTIONSYSTEM_H
