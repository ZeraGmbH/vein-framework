#ifndef VF_RPC_INVOKER_H
#define VF_RPC_INVOKER_H

#include "vf_entity_event_item.h"
#include "vfrpceventgenerator.h"
#include <QObject>

class VfRPCInvoker : public QObject, public VfEntityEventItem
{
    Q_OBJECT
public:
    static std::shared_ptr<VfRPCInvoker> create(int entityId, std::unique_ptr<VfRPCEventGenerator> invokerType);
    explicit VfRPCInvoker(int entityId, std::unique_ptr<VfRPCEventGenerator> invokerType);
    QUuid invokeRPC(QString procedureName, QVariantMap paramters);

signals:
    void sigRPCFinished(bool ok, QUuid identifier, const QVariantMap &resultData);

private:
    void processCommandEvent(VeinEvent::CommandEvent *cmdEvent) override;
    void processErrorCommandEventData(VeinEvent::EventData *originalEventData) override;

    QSet<QUuid> m_pendingRPCs;
    std::unique_ptr<VfRPCEventGenerator> m_invokerType;
};
typedef std::shared_ptr<VfRPCInvoker> VfRPCInvokerPtr;
#endif // VF_RPC_INVOKER_H
