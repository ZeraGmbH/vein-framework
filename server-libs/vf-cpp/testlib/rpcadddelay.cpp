#include "rpcadddelay.h"
#include "vf-cpp-rpc-signature.h"

RpcAddDelay::RpcAddDelay(VeinEvent::EventSystem *eventSystem, int entityId) :
        VfCpp::VfCppRpcSimplified(eventSystem,
                                    entityId,
                                    VfCpp::VfCppRpcSignature::createRpcSignature("RPC_addDelay", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}, {"p_delayMs", "int"}})))
{
}

void RpcAddDelay::callRPCFunction(const QUuid &callId, const QVariantMap &parameters)
{
    RPC_addDelay(callId, parameters);
}

void RpcAddDelay::RPC_addDelay(QUuid callId, QVariantMap parameters)
{
    int input = parameters["p_param"].toInt();
    int delayMs = parameters["p_delayMs"].toInt();

    if(delayMs > 0) {
        m_delayResponse = TimerFactoryQt::createSingleShot(delayMs);
        connect(m_delayResponse.get(), &TimerTemplateQt::sigExpired, this, [callId, input, this](){
            sendRpcResult(callId, QString::number(input));
        });
        m_delayResponse->start();
    }
    else
        sendRpcError(callId, "Error");
}
