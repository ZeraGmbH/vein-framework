#include "vf-cpp-rpc-simplified.h"
#include "ve_commandevent.h"
#include "vf_server_rpc_register.h"
#include "vf_server_rpc_result.h"
#include "vcmp_remoteproceduredata.h"

using namespace VfCpp;

VfCppRpcSimplified::VfCppRpcSimplified(VeinEvent::EventSystem *eventsystem, int entityId, QString rpcSignature) :
    m_eventSystem(eventsystem),
    m_entityId(entityId),
    m_rpcSignature(rpcSignature)
{
    emit m_eventSystem->sigSendEvent(VfServerRpcRegister::generateEvent(m_entityId, rpcSignature));
    connect(this, &VfCppRpcSimplified::callRpc, this, &VfCppRpcSimplified::callRPCFunction, Qt::QueuedConnection);
}

QString VfCppRpcSimplified::getSignature()
{
    return m_rpcSignature;
}

void VfCppRpcSimplified::callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters)
{
    QVariantMap params = parameters.value(VeinComponent::RemoteProcedureData::s_parameterString).toMap();
    m_callIdPeerIdHash.insert(callId, peerId);
    emit callRpc(callId, peerId, params);
}

void VfCppRpcSimplified::sendRpcResult(const QUuid &callId, QVariant result)
{
    QVariantMap returnVal;
    returnVal.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
    returnVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_SUCCESS);
    if(!result.isNull()) {
        returnVal.insert(VeinComponent::RemoteProcedureData::s_returnString, result);
    }
    sendRPCresultEvent(callId, returnVal);
}

void VfCppRpcSimplified::sendRpcError(const QUuid &callId, QString errorStr)
{
    QVariantMap returnVal;
    returnVal.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
    returnVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_EINVAL);
    if(!errorStr.isEmpty()) {
        returnVal.insert(VeinComponent::RemoteProcedureData::s_errorMessageString, errorStr);
    }
    sendRPCresultEvent(callId, returnVal);
}

void VfCppRpcSimplified::sendRPCresultEvent(const QUuid &callId, QVariantMap returnVal)
{
    if(m_callIdPeerIdHash.contains(callId)) {
        VeinEvent::CommandEvent *rpcResultEvent = VfServerRpcResult::generateEvent(m_entityId, m_rpcSignature, returnVal);
        rpcResultEvent->setPeerId(m_callIdPeerIdHash.value(callId));
        m_callIdPeerIdHash.remove(callId);
        emit m_eventSystem->sigSendEvent(rpcResultEvent);
    }
}
