#ifndef VF_CLIENT_RPC_INVOKER_H
#define VF_CLIENT_RPC_INVOKER_H

#include <QObject>
#include <vf_entity_event_item.h>

class VfClientRPCInvoker : public QObject, public VfEntityEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfClientRPCInvoker> create(int entityId);
    VfClientRPCInvoker(int entityId);
    void invokeRPC(QString procedureName, QVariantMap paramters);
signals:
    void sigRPCFinished(bool ok, QUuid identifier, const QVariantMap &resultData);

private:
    static QEvent *generateEvent(int entityId, QString procedureName, QVariantMap parameters);
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;
};
typedef std::shared_ptr<VfClientRPCInvoker> VfClientRPCInvokerPtr;

#endif // VF_CLIENT_RPC_INVOKER_H
