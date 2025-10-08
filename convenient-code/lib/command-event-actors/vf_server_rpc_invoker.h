#ifndef VFSERVERRPCINVOKER_H
#define VFSERVERRPCINVOKER_H

#include "vf_entity_event_item.h"
#include <QObject>

class VfServerRPCInvoker : public QObject, public VfEntityEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfServerRPCInvoker> create(int entityId);
    explicit VfServerRPCInvoker(int entityId);
    QUuid invokeRPC(QString procedureName, QVariantMap paramters);
signals:
    void sigRPCFinished(bool ok, QUuid identifier, const QVariantMap &resultData);

private:
    static QEvent *generateEvent(int entityId, QString procedureName, QVariantMap parameters, QUuid identifier);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;

    QSet<QUuid> m_pendingRPCs;
};
typedef std::shared_ptr<VfServerRPCInvoker> VfServerRPCInvokerPtr;

#endif // VFSERVERRPCINVOKER_H
