#include "vf-cpp-rpc-simplified.h"
#include "vf_server_rpc_register.h"

VfCppRpcSimplified::VfCppRpcSimplified(VeinEvent::EventSystem *eventsystem, int entityId, QString rpcName, VfCpp::VfCppRpcSignature::RPCParams parameters) :
    m_eventSystem(eventsystem),
    m_entityId(entityId)
{
    m_rpcSignature = VfCpp::VfCppRpcSignature::createRpcSignature(rpcName, parameters);
    emit m_eventSystem->sigSendEvent(VfServerRpcRegister::generateEvent(m_entityId, m_rpcSignature));
}

QString VfCppRpcSimplified::getSignature()
{
    return m_rpcSignature;
}

void VfCppRpcSimplified::callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters)
{

}
