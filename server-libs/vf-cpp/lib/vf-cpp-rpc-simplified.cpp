#include "vf-cpp-rpc-simplified.h"
#include "vf_server_rpc_register.h"
#include "vf_server_rpc_result.h"
#include "vcmp_remoteproceduredata.h"

using namespace VfCpp;

VfCppRpcSimplified::VfCppRpcSimplified(QObject *object, VeinEvent::EventSystem *eventsystem, int entityId, QString rpcName, VfCppRpcSignature::RPCParams parameters) :
    m_object(object),
    m_eventSystem(eventsystem),
    m_entityId(entityId),
    m_rpcName(rpcName)
{
    m_rpcSignature = VfCppRpcSignature::createRpcSignature(m_rpcName, parameters);
    emit m_eventSystem->sigSendEvent(VfServerRpcRegister::generateEvent(m_entityId, m_rpcSignature));
}

QString VfCppRpcSimplified::getSignature()
{
    return m_rpcSignature;
}

void VfCppRpcSimplified::callFunction(const QUuid &callId, const QUuid &peerId, const QVariantMap &parameters)
{
    QVariantMap params = parameters.value(VeinComponent::RemoteProcedureData::s_parameterString).toMap();
    bool methodFound = QMetaObject::invokeMethod(m_object, m_rpcName.toUtf8(), Qt::DirectConnection, Q_ARG(QUuid, callId), Q_ARG(QVariantMap, params));
    if(!methodFound)
        sendRpcError(callId, "Function not implemented/accessible");
}

void VfCppRpcSimplified::sendRpcResult(const QUuid &callId, QVariant result)
{
    QVariantMap returnVal;
    returnVal.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
    returnVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_SUCCESS);
    if(!result.isNull()) {
        returnVal.insert(VeinComponent::RemoteProcedureData::s_returnString, result);
    }
    emit m_eventSystem->sigSendEvent(VfServerRpcResult::generateEvent(m_entityId, m_rpcSignature, returnVal));
}

void VfCppRpcSimplified::sendRpcError(const QUuid &callId, QString errorStr)
{
    QVariantMap returnVal;
    returnVal.insert(VeinComponent::RemoteProcedureData::s_callIdString, callId);
    returnVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, VeinComponent::RemoteProcedureData::RPCResultCodes::RPC_EINVAL);
    if(!errorStr.isEmpty()) {
        returnVal.insert(VeinComponent::RemoteProcedureData::s_errorMessageString, errorStr);
    }
    emit m_eventSystem->sigSendEvent(VfServerRpcResult::generateEvent(m_entityId, m_rpcSignature, returnVal));
}
