#ifndef RPCFORTEST_H
#define RPCFORTEST_H

#include "vf-cpp-rpc-simplified.h"
#include <QUuid>

class RpcForTest : public VfCpp::VfCppRpcSimplified
{
public:
    RpcForTest(VeinEvent::EventSystem *eventSystem, int entityId);
private slots:
    void callRPCFunction(const QUuid &callId, const QVariantMap &parameters) override;
private:
    void RPC_forTest(QUuid callId, QVariantMap parameters);
};

#endif // RPCFORTEST_H
