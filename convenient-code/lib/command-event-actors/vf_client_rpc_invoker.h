#ifndef VF_CLIENT_RPC_INVOKER_H
#define VF_CLIENT_RPC_INVOKER_H

#include "vfrpceventgenerator.h"
#include <memory>
#include <QObject>

class VfClientRPCInvoker : public VfRPCEventGenerator
{
    Q_OBJECT
protected:
    QEvent *generateEvent(int entityId, QString procedureName, QVariantMap parameters, QUuid identifier) override;
};
typedef std::unique_ptr<VfClientRPCInvoker> VfClientRPCInvokerPtr;

#endif // VF_CLIENT_RPC_INVOKER_H
