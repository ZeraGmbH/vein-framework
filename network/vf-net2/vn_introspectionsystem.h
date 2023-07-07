#ifndef VEINNET_INTROSPECTIONSYSTEM_H
#define VEINNET_INTROSPECTIONSYSTEM_H

#include <ve_eventsystem.h>
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

    void processEvent(QEvent *event) override;
private:
    QJsonObject getJsonIntrospection(int entityId) const;
    QHash<int, EntityIntrospection*> m_introspectionData;
};

} // namespace VeinNet

#endif // VEINNET_INTROSPECTIONSYSTEM_H
