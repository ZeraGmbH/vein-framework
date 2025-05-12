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
        m_entity->createRpc(this, "RPC_PrivateMethod", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}}));
        m_entity->createRpc(this, "RPC_PublicMethod", VfCpp::VfCppRpcSignature::RPCParams({{"p_param", "int"}}));
    }
}

void VfTestRpcSimplified::RPC_PublicMethod(QVariantMap parameters)
{
}

void VfTestRpcSimplified::RPC_forTest(QVariantMap parameters)
{
    QUuid callId = parameters[VeinComponent::RemoteProcedureData::s_callIdString].toUuid();
    int input = parameters["p_param"].toInt();
    if(input > 0)
        m_entity->sendRpcResult(callId, "RPC_forTest", QString::number(input));
    else
        m_entity->sendRpcError(callId, "RPC_forTest", "Error");
}

void VfTestRpcSimplified::RPC_PrivateMethod(QVariantMap parameters)
{
}
