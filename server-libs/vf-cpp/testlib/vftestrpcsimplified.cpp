#include "vftestrpcsimplified.h"
#include "vf-cpp-rpc-signature.h"
#include "vcmp_remoteproceduredata.h"

VfTestRpcSimplified::VfTestRpcSimplified(int entityId)
{
    m_entity = std::make_shared<VfEntityWithRpcSimplified>(entityId);
    m_initialized = false;
}

VfEntityWithRpcSimplified *VfTestRpcSimplified::getEntity()
{
    return m_entity.get();
}

void VfTestRpcSimplified::initOnce()
{
    if(!m_initialized) {
        m_initialized = true;
        m_entity->initModule();
        m_entity->createRpc(this, "RPC_forTest", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}}));
        m_entity->createRpc(this, "RPC_addDelay", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}, {"p_delayMs", "int"}}));
        m_entity->createRpc(this, "RPC_PrivateMethod", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}}));
        m_entity->createRpc(this, "RPC_PublicMethod", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}}));
    }
}

void VfTestRpcSimplified::RPC_PublicMethod(QUuid callId, QVariantMap parameters)
{
}

void VfTestRpcSimplified::RPC_forTest(QUuid callId, QVariantMap parameters)
{
    int input = parameters["p_param"].toInt();
    if(input > 0)
        m_entity->sendRpcResult(callId, "RPC_forTest", QString::number(input));
    else
        m_entity->sendRpcError(callId, "RPC_forTest", "Error");
}

void VfTestRpcSimplified::RPC_addDelay(QUuid callId, QVariantMap parameters)
{
    int input = parameters["p_param"].toInt();
    int delayMs = parameters["p_delayMs"].toInt();

    if(delayMs > 0) {
        m_delayResponse = TimerFactoryQt::createSingleShot(delayMs);
        connect(m_delayResponse.get(), &TimerTemplateQt::sigExpired, this, [callId, input, this](){
            m_entity->sendRpcResult(callId, "RPC_addDelay", QString::number(input));
        });
        m_delayResponse->start();
    }
    else
        m_entity->sendRpcError(callId, "RPC_addDelay", "Error");
}

void VfTestRpcSimplified::RPC_PrivateMethod(QUuid callId, QVariantMap parameters)
{
}
