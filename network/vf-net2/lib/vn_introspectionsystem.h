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
    explicit IntrospectionSystem(QObject *parent=nullptr);
    static const QString s_nameComponent;
    void setStorage(VeinStorage::AbstractEventSystem *storageSystem);

    void processEvent(QEvent *event) override;
private:
    QJsonObject getJsonIntrospection(int entityId) const;
    QStringList getComonentNames(int entityId) const;
    QStringList getRpcNames(int entityId) const;
    QHash<int, EntityIntrospection*> m_introspectionData;
    VeinStorage::AbstractEventSystem *m_storageSystem = nullptr;
};

} // namespace VeinNet

#endif // VEINNET_INTROSPECTIONSYSTEM_H
