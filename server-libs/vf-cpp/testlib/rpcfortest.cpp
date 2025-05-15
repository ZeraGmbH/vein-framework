#include "rpcfortest.h"
#include "vf-cpp-rpc-signature.h"

RpcForTest::RpcForTest(VeinEvent::EventSystem *eventSystem, int entityId) :
    VfCpp::VfCppRpcSimplified(eventSystem,
                                entityId,
                                VfCpp::VfCppRpcSignature::createRpcSignature("RPC_forTest", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}})))
{
}

void RpcForTest::callRPCFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters)
{
    RPC_forTest(callId, parameters);
}

void RpcForTest::RPC_forTest(QUuid callId, QVariantMap parameters)
{
    int input = parameters["p_param"].toInt();
    if(input > 0)
        sendRpcResult(callId, QString::number(input));
    else
        sendRpcError(callId, "Error");
}
