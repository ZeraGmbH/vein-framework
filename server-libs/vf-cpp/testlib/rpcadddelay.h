#ifndef RPCADDDELAY_H
#define RPCADDDELAY_H

#include "vf-cpp-rpc-simplified.h"
#include <timerfactoryqt.h>
#include <QUuid>

class RpcAddDelay : public VfCpp::VfCppRpcSimplified
{
public:
    RpcAddDelay(VeinEvent::EventSystem *eventSystem, int entityId);
private slots:
    void callRPCFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters) override;
private:
    void RPC_addDelay(QUuid callId, QVariantMap parameters);
    TimerTemplateQtPtr m_delayResponse;
};

#endif // RPCADDDELAY_H
